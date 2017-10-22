#include "Wire.h"

#define I2C_ADDR 0x2f
#define BAUD     9600

void setup() {
  Serial.begin(BAUD);
  Wire.begin(I2C_ADDR);
  Wire.onReceive(receiveEvent);
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
  while (Wire.available() > 1) {
    char c = Wire.read();
    Serial.print(c);
  }

  int x = Wire.read();
  Serial.println(x);
}

