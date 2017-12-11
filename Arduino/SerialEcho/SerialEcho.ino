#define BAUD    9600

void setup() {
  Serial.begin(BAUD);
  while (!Serial);
}

void loop() {
  if (Serial.available() > 0) {
    Serial.print((char) Serial.read());
  }
}
