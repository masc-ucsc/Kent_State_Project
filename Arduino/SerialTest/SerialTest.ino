#define BAUD            9600
#define PWR_LIGHT_PIN   4

#define POWER_ON_CMD    "pon"
#define POWER_OFF_CMD   "off"

#define BUFFER_SIZE     64

char serialInBuffer[BUFFER_SIZE];
int ibIndex = 0;
bool ibEmpty = true;

void setup() {
  Serial.begin(BAUD);         // default settings: 8 data bits, no parity, one stop
  while (!Serial);

  pinMode(PWR_LIGHT_PIN, OUTPUT);
  digitalWrite(PWR_LIGHT_PIN, LOW);
}

void loop() {
  if (Serial.available()) {
    int c = Serial.read();

    if (c == '\n') {
      processIBuffer();
      ibEmpty = true;
    } else if (ibIndex < BUFFER_SIZE) {
      ibEmpty = false;
      serialInBuffer[ibIndex++] = c;
    }
  }
}

void processIBuffer() {
  if (strncmp(serialInBuffer, POWER_ON_CMD, 3) == 0) {
    digitalWrite(PWR_LIGHT_PIN, HIGH);
  } else if (strncmp(serialInBuffer, POWER_OFF_CMD, 3) == 0) {
    digitalWrite(PWR_LIGHT_PIN, LOW);
  }

  ibIndex = 0;
}

