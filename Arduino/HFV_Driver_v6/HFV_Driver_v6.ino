#include "Wire.h"

//#define DISABLE_I2C

#define POT_ADDR      0x2f
#define BAUD 9600
#define VOUT_PIN      5

#define BUFFER_SIZE 256
char inBuffer[BUFFER_SIZE];
int ibIndex = 0;

#define STATE_OFF     0
#define STATE_ON      1
#define STATE_RUNNING 2
#define STATE_FAULT   3
int state = STATE_OFF;

uint64_t lastUpdate = 0;
#define UPDATE_INTERVAL         1000

uint64_t lastPulseUpdate = 0;
int pulseState = STATE_OFF;

int voltage = 0;
int dutyCycle = 0;
int pulseTime = 0;
int pulseSpacing = 0;

bool voltageChanged = false;
bool dutyCycleChanged = false;

void setup() {
  Serial.begin(BAUD);         // default settings: 8 data bits, no parity, one stop
  while (!Serial);
  pinMode(VOUT_PIN, OUTPUT);
  digitalWrite(VOUT_PIN, LOW);

#ifndef DISABLE_I2C
  Wire.begin();               // I2C
#endif
}

void loop() {
  uint64_t currentTime = millis();
  
  if (Serial.available()) {
    int c = Serial.read();

    if (state != STATE_FAULT) {
      if (c == '\n') {
        inBuffer[ibIndex] = '\0';
        ibIndex = 0;

        processCommand(currentTime);
        
      } else {
        if (ibIndex >= BUFFER_SIZE)
          ibIndex = 0;
        else
          inBuffer[ibIndex++] = (char) c;
      }
    }
  }

  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    if (state == STATE_ON)
      updateOutput(currentTime);
    message();
    lastUpdate = currentTime;
  }
}

void updateOutput(uint64_t currentTime) {
  if (pulseState == STATE_OFF) {
    if (currentTime - lastPulseUpdate > pulseSpacing) {
      pulseState = STATE_ON;
      outputOn();
      lastPulseUpdate = currentTime;
    }
  } else {    
    if (currentTime - lastPulseUpdate > pulseTime) {
      pulseState = STATE_OFF;
      outputOff();
      lastPulseUpdate = currentTime;
    }
  }

  if (voltageChanged) {
    if (voltage > 127 || voltage < 0)
      voltage = 0;

#ifndef DISABLE_I2C
    Wire.beginTransmission(POT_ADDR);
    Wire.write(voltage);
    Wire.endTransmission();
#endif

    voltageChanged = false;
  }

  if (dutyCycleChanged) {
    pwmOn();
    dutyCycleChanged = false;
  }
}

void outputOff() {
  pwmOff();
  digitalWrite(VOUT_PIN, LOW);
}

void outputOn() {
  if (dutyCycle > 0 && dutyCycle <= 256) {
    pwmOn();
  } else {
    pwmOff();
    digitalWrite(VOUT_PIN, HIGH);
  }
}

void pwmOff() {
  TCCR0A &= ~((1 << WGM01) | (1 << WGM00));
}

void pwmOn() {
  OCR0A  = dutyCycle;
  TCCR0A |= (1 << COM0A1);                // non-inverting
  TCCR0A |= (1 << WGM01) | (1 << WGM00);  // fast pwm
}

void processCommand(uint64_t currentTime) {
  if (inBuffer[0] == 'v') {
    voltage = modified_atoi(&inBuffer[1]);
    voltageChanged = true;
  } else if (inBuffer[0] == 'd') {
    dutyCycle = modified_atoi(&inBuffer[1]);
    dutyCycleChanged = true;
  } else if (inBuffer[0] == 'p')
    pulseTime = modified_atoi(&inBuffer[1]);
  else if (inBuffer[0] == 's')
    pulseSpacing = modified_atoi(&inBuffer[1]);
  else if (inBuffer[0] == 'o' && inBuffer[1] == 'n' && inBuffer[2] == 'n') {
    state = STATE_ON;
    lastPulseUpdate = currentTime;
    pulseState = STATE_OFF;

    outputOn();
  } else if (inBuffer[0] == 'o' && inBuffer[1] == 'f' && inBuffer[2] == 'f') {
    state = STATE_OFF;
    outputOff();
  }
}

void message() {
  switch (state) {
    case STATE_OFF:
      Serial.print("off");
      break;

    case STATE_ON:
      Serial.print("onn");
      break;

    default:
      Serial.print("flt");
  }

  Serial.print(";");
  Serial.print(voltage);
  Serial.print(";");
  Serial.print(dutyCycle);
  Serial.print(";");
  Serial.print(pulseTime);
  Serial.print(";");
  Serial.print(pulseSpacing);
  Serial.print("\n");
}

int modified_atoi(char *buf) {
  int accum = 0;

  for (int i = 0; buf[i] >= '0' && buf[i] <= '9'; i++)
    accum = (accum * 10) + (buf[i] - '0');

  return accum;
}

