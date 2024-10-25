#include <Arduino.h>
#define LED_RED 28
#define LED_ORANGE 29
#define LED_GREEN 30
#define LED_BLUE 31


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_RED, HIGH);
  delay(1000);
  digitalWrite(LED_RED, LOW);
  delay(1000);
  digitalWrite(LED_ORANGE, HIGH);
  delay(1000);
  digitalWrite(LED_ORANGE, LOW);
  delay(1000);
  digitalWrite(LED_GREEN, HIGH);
  delay(1000);
  digitalWrite(LED_GREEN, LOW);
  delay(1000);
  digitalWrite(LED_BLUE, HIGH);
  delay(1000);
  digitalWrite(LED_BLUE, LOW);
  delay(1000);
}
