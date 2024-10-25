#include <Arduino.h>
#define LED_RED 28
#define LED_ORANGE 29
#define LED_GREEN 30
#define LED_BLUE 31
#include <Wire.h>
#define POTENTIOMETER_A 0x2C
#define POTENTIOMETER_B 0x2D


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C transmission
  Wire.beginTransmission(POTENTIOMETER_A);
  // Send instruction for POT channel-0
  Wire.write(0x01);
  // Input resistance value, 0x80(128)
  Wire.write(0x80);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C transmission
  Wire.beginTransmission(POTENTIOMETER_A);
  // Send instruction for POT channel-1
  Wire.write(0x03);
  // Input resistance value, 0x80(128)
  Wire.write(0x80);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
}

void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(LED_RED, HIGH);
  // delay(100);
  // digitalWrite(LED_RED, LOW);
  // delay(100);
  // digitalWrite(LED_ORANGE, HIGH);
  // delay(100);
  // digitalWrite(LED_ORANGE, LOW);
  // delay(100);
  // digitalWrite(LED_GREEN, HIGH);
  // delay(100);
  // digitalWrite(LED_GREEN, LOW);
  // delay(100);
  // digitalWrite(LED_BLUE, HIGH);
  // delay(100);
  // digitalWrite(LED_BLUE, LOW);
  // delay(100);

  Wire.beginTransmission(POTENTIOMETER_A);
  // Select data register
  Wire.write(0x01);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(POTENTIOMETER_A, 1);
  int data;
  // Read 1 byte of data
  if (Wire.available() == 1)
  {
    data = Wire.read();
  }

  // Convert the data
  float res_1 = (data / 256.0 ) * 10.0;

  // Start I2C transmission
  Wire.beginTransmission(POTENTIOMETER_A);
  // Select data register
  Wire.write(0x03);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 1 byte of data
  Wire.requestFrom(POTENTIOMETER_A, 1);

  // Read 1 byte of data
  if (Wire.available() == 1)
  {
    data = Wire.read();
  }

  // Convert the data
  float res_2 = (data / 256.0 ) * 10.0;

  // Output data to serial monitor
  Serial.print("Resistance Channel-0 : ");
  Serial.print(res_1);
  Serial.println(" K");
  Serial.print("Resistance Channel-1 : ");
  Serial.print(res_2);
  Serial.println(" K");
  delay(500);
}
