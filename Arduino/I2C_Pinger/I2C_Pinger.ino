#include "Wire.h"

#define I2C_ADDR        0x2f
#define BAUD            9600
#define SEND_INTERVAL   2000

uint64_t lastSend = 0;
int counter = 0;

void setup() {
  Wire.begin();
}

void loop() {
  uint64_t currentTime = millis();

  if (currentTime - lastSend > SEND_INTERVAL) {
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(counter);
    Wire.endTransmission();
    counter++;
    lastSend = currentTime;
  }

  delay(30);
}
