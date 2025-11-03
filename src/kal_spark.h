#include <stdint.h>
#include <Arduino.h>
// Kamaji abstraction layer code for SPARK
// Every hardware will have its own abstraction layer but they will follow the same notation.

bool k_check_efuse_reset() {
    // Checks serial for if the efuse reset command (#r) is received.
    if(Serial.available() == 2) {
        uint8_t buf[2];
        Serial.readBytes((char*) &buf, 2);

        if(buf[0] == '#' && buf[1] == 'r') {
            return true;
        }
    }
    return false;
}

void k_report(uint8_t state) {
    // Reports the fire state to kamaji in form of (%d + [state])
    // Because simulation timestamps are synced to MIDAS's time, we don't care what the time is on this system.
    // As such we omit the 4 timestamp bytes, the event type is one char, and every SPARK event will have 1 byte of data.
    uint8_t buf[3] = {'%', 'd', state};
    Serial.write(buf, 3);
}

void k_trigger(uint8_t channel) {
    // Read above, except the event type is now 't' for "trigger", and the data byte is the channel which triggered.
    uint8_t buf[3] = {'%', 't', channel};
    Serial.write(buf, 3);
}

void k_setup_done() {
    // Don't care about data here, 's' means setup is done.
    uint8_t buf[2] = {'%', 's'};
    Serial.write(buf, 2);
}

inline void k_wait_until(char sig) {
    char a;
    do {
        a = Serial.read();
    } while (a != sig);
}

inline void k_clear_inbuf() {
    while(Serial.available() > 0) {
        char tb = Serial.read();
    }
}