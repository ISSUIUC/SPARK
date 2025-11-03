#include <Arduino.h>
#include "pins.h"
#include "Wire.h"
#include "driver_func.h"
#include "kal_spark.h"

AD5252 pot_a {Wire1, POT_A};
AD5252 pot_b {Wire1, POT_B};
EFuseCollection efuses {EFUSE_A_PG, EFUSE_B_PG, EFUSE_C_PG, EFUSE_D_PG};

void efuse_tripped(PyroChannel channel) {
  k_trigger((uint8_t) channel);

  #ifndef HITL
  tone(BUZZER_PIN, 2730);
  delay(100);
  noTone(BUZZER_PIN);
  #endif
}

void setup() {
  Serial.begin(9600);
  #ifdef HITL
  while(!Serial);
  k_wait_until('\n');
  k_clear_inbuf();
  #endif

  // Begin pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(EFUSE_A_PG, INPUT);
  pinMode(EFUSE_B_PG, INPUT);
  pinMode(EFUSE_C_PG, INPUT);
  pinMode(EFUSE_D_PG, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  Wire1.begin();

  digitalWrite(LED_ORANGE, HIGH);

  pot_a.setRDACValue(AD5252RDAC::RDAC1, 0xA0);
  pot_a.setRDACValue(AD5252RDAC::RDAC3, 0xA0);
  pot_b.setRDACValue(AD5252RDAC::RDAC1, 0xA0);
  pot_b.setRDACValue(AD5252RDAC::RDAC3, 0xA0);

  // Sing!
  constexpr int s_delay = 35;
  for(int i = 0; i < 10; i++) {
    tone(BUZZER_PIN, 2730);
    delay(s_delay);
    tone(BUZZER_PIN, 2950);
    delay(s_delay);
  }
  noTone(BUZZER_PIN);

  efuses.SetTriggerCallback(efuse_tripped);
  efuses.reset();

  delay(50);
  digitalWrite(LED_ORANGE, LOW);
}

int i = 0;
void loop() {
  efuses.tick();
  i++;
  digitalWrite(LED_RED, efuses.tripped(PyroChannel::PYRO_A) ? LOW : HIGH);
  digitalWrite(LED_ORANGE, efuses.tripped(PyroChannel::PYRO_B) ? LOW : HIGH);
  digitalWrite(LED_GREEN, efuses.tripped(PyroChannel::PYRO_C) ? LOW : HIGH);
  digitalWrite(LED_BLUE, efuses.tripped(PyroChannel::PYRO_D) ? LOW : HIGH);


  if(i > 50) {
    i = 0;
    // Handle input
    k_check_efuse_reset();
    
    // Report
    k_report(efuses.encode());
  }

  delay(2);
}
