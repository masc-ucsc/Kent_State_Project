#include <SPI.h>
#include <stdlib.h>
#define BAUD 9600

#define CS_PIN            10
#define WIPER_VOLTAGE_PIN 1

void setup() {
  pinMode(CS_PIN, OUTPUT);
  Serial.begin(BAUD);
  SPI.begin();
}

void loop() {
  static int rawVoltage = 0;
  static int lastRawVoltage = 0;
  processSerial();

  rawVoltage = analogRead(WIPER_VOLTAGE_PIN);

  int diff = abs(rawVoltage - lastRawVoltage);
  
  if (diff > 5) {
    Serial.print("Voltage: ");
    Serial.println(rawVoltage);
    lastRawVoltage = rawVoltage;
  }
}

void processSerial() {
  static char cmdBuffer[256];
  static int index = 0;

  if (Serial.available()) {
    int c = Serial.read();

    if (c == '\n') {
      cmdBuffer[index] = '\0';
      byte level = atoi(cmdBuffer);
      index = 0;

      writePOTValue(level);
    } else {
      cmdBuffer[index++] = c;
    }
  }
}

void writePOTValue(byte level) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(B00010001);
  SPI.transfer(level);
  digitalWrite(CS_PIN, HIGH);
}

