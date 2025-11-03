#pragma once
// Super minimal c++-style driver for the AD5252 and the TPS25947
// Kamaji-style event reporting for HITL
#include <Wire.h>
#include <Arduino.h>

enum AD5252RDAC {
    RDAC1 = 0x01,
    RDAC3 = 0x03,
};

class AD5252 {
    public:
    AD5252(TwoWire& i2c, uint8_t addr) {
        _i2c = &i2c;
        _i2caddr = addr;
    }

    void setRDACValue(AD5252RDAC rdac_register, uint8_t r_value) {
        _begini2c();
        _i2c->write((uint8_t)rdac_register);
        _i2c->write(r_value);
        _endi2c();
        delay(50);
    }

    uint8_t read_register(AD5252RDAC reg) {
        _begini2c();
        _i2c->write((uint8_t)reg);
        _endi2c();

        _i2c->requestFrom(_i2caddr, 1);
        if(_i2c->available() == 1) {
            return _i2c->read();
        } else {
            return -1;
        }
    }

    float read_register_f(AD5252RDAC reg) {
        uint8_t data = read_register(reg);
        return (data / 256.0 ) * 10.0;
    }


    private:
    TwoWire* _i2c;
    uint8_t _i2caddr;
    void _begini2c() { _i2c->beginTransmission(_i2caddr); }
    void _endi2c() { _i2c->endTransmission(); }
};

enum PyroChannel {
    PYRO_A = 0,
    PYRO_B = 1,
    PYRO_C = 2,
    PYRO_D = 3,
};

class EFuseCollection {
    private:
    uint8_t _trigger_pins[4];
    uint8_t _trigger_states[4]; // 0 -> not fired, 1 -> fired
    uint8_t _states[4]; // raw pg value
    void (*_trigger_callback)(PyroChannel);

    uint8_t _read(uint8_t idx) {
        return digitalRead(_trigger_pins[idx]);
    }

    public:
    EFuseCollection(uint8_t t1, uint8_t t2, uint8_t t3, uint8_t t4) {
        _trigger_pins[0] = t1;
        _trigger_pins[1] = t2;
        _trigger_pins[2] = t3;
        _trigger_pins[3] = t4;
    }

    void SetTriggerCallback(void (*_cb)(PyroChannel)) {
        _trigger_callback = _cb;
    }

    void reset() {
        for(uint8_t i = 0; i < 4; i++) {
            _trigger_states[i] = 0;
            _states[i] = _read(i);
        }
    }

    void tick() {
        for(uint8_t i = 0; i < 4; i++) {
            uint8_t cur_state = _read(i);
            if(_states[i] == 1 && cur_state == 0) {
                // Fired
                _trigger_states[i] = 1;
                _trigger_callback((PyroChannel) i);
            }
            _states[i] = cur_state;
        }
    }

    bool continuous(PyroChannel pc) { return _states[(uint8_t)pc]; }
    bool tripped(PyroChannel pc) { return _trigger_states[(uint8_t)pc]; }

    uint8_t encode() {
        // Returns state as a set of 2 nybbles representing A/B/C/D states.
        // First nybble: "continuous" --> PG state
        uint8_t state = 0;
        for(uint8_t i = 0; i < 4; i++) {
            state = (state << 1) + (_states[i] & 0x1);
        }

        // Next nybble: "tripped"
        for(uint8_t i = 0; i < 4; i++) {
            state = (state << 1) + (_trigger_states[i] & 0x1);
        }
    }
};