
#define BAUD 9600

#define TURN_ON         "pon"
#define TURN_OFF        "off"
#define VOLTAGE_UP      "vup"
#define VOLTAGE_DOWN    "vdn"
#define DUTY_CYCLE_UP   "dup"
#define DUTY_CYCLE_DOWN "ddn"

#define BUFFER_SIZE 16
char inBuffer[BUFFER_SIZE];
int ibIndex = 0;

#define STATE_OFF     0
#define STATE_ON      1
#define STATE_FAULT   2
int state = STATE_OFF;

uint64_t lastUpdate = 0;
#define UPDATE_INTERVAL         1000

// debug values
int voltage = 0;
int dutyCycle = 0;

void setup() {
  Serial.begin(BAUD);         // default settings: 8 data bits, no parity, one stop
  while (!Serial);
}

void loop() {
  if (Serial.available()) {
    int c = Serial.read();

    if (state != STATE_FAULT) {
      if (c == '\n') {
        inBuffer[ibIndex] = '\0';
        ibIndex = 0;
        
        processCommand();
        
      } else {
        if (ibIndex >= BUFFER_SIZE)
          state = STATE_FAULT;
        else
          inBuffer[ibIndex++] = (char) c;
      }
    }
  }

  uint64_t currentTime = millis();
  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    message();
    lastUpdate = currentTime;
  }
}

void processCommand() {
  if (inBuffer[0] == 'v')
    voltage = modified_atoi(&inBuffer[1]);
  else if (inBuffer[0] == 'd')
    dutyCycle = modified_atoi(&inBuffer[1]);
  else if (inBuffer[0] == 'o' && inBuffer[1] == 'n' && inBuffer[2] == 'n')
    state = STATE_ON;
  else if (inBuffer[0] == 'o' && inBuffer[1] == 'f' && inBuffer[2] == 'f')
    state = STATE_OFF;
    
  /*if (strcmp(inBuffer, SEND_STATUS) == 0) {
    // do nothing
  } else if (strcmp(inBuffer, VOLTAGE_UP) == 0) {
    voltage++;
  } else if (strcmp(inBuffer, VOLTAGE_DOWN) == 0) {
    voltage--;
  } else if (strcmp(inBuffer, DUTY_CYCLE_UP) == 0) {
    dutyCycle++;
  } else if (strcmp(inBuffer, DUTY_CYCLE_DOWN) == 0) {
    dutyCycle--;
  } else if (strcmp(inBuffer, TURN_ON) == 0) {
    state = STATE_ON;
  } else if (strcmp(inBuffer, TURN_OFF) == 0) {
    state = STATE_OFF;
  } else {
    state = STATE_FAULT;
  }*/
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
  Serial.print("\n");
}

int modified_atoi(char *buf) {
  int accum = 0;

  for (int i = 0; buf[i] >= '0' && buf[i] <= '9'; i++)
    accum = (accum * 10) + (buf[i] - '0');

  return accum;
}

