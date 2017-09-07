#define VUP_BUTTON    4
#define VDN_BUTTON    7
#define PON_BUTTON    2
#define CSn_PIN       1
#define INCn_PIN      5
#define DIRECTION_PIN 3

#define BAUD          9600
#define POT_DELAY     5       // ms

bool ponSeenPressed = false;
bool vupSeenPressed = false;
bool vdnSeenPressed = false;

void setup() {
  pinMode(VUP_BUTTON, INPUT);
  pinMode(VDN_BUTTON, INPUT);
  pinMode(PON_BUTTON, INPUT);

  pinMode(CSn_PIN, OUTPUT);
  pinMode(INCn_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);

  digitalWrite(CSn_PIN, HIGH);
  digitalWrite(INCn_PIN, HIGH);

  Serial.begin(BAUD);
  while (!Serial);
}

void loop() {
  if (processButton(VUP_BUTTON, &vupSeenPressed)) {
    Serial.println("up pressed");
    potChange(true);
  }
  
  if (processButton(VDN_BUTTON, &vdnSeenPressed)) {
    Serial.println("down pressed");
    potChange(false);
  }
  
  processButton(PON_BUTTON, &ponSeenPressed);
  
  delay(30);
}

bool processButton(int pin, bool *seenFlag) {
  int val = digitalRead(pin);

  if (val == HIGH && !(*seenFlag)) {
    *seenFlag = true;
    return true;
  } else if (val == LOW) {
    *seenFlag = false;
  }

  return false;
}

void potChange(bool up) {
  digitalWrite(CSn_PIN, LOW);
  delay(POT_DELAY);

  digitalWrite(DIRECTION_PIN, (up) ? HIGH : LOW);
  delay(POT_DELAY);

  digitalWrite(INCn_PIN, LOW);
  delay(POT_DELAY);

  digitalWrite(INCn_PIN, HIGH);
  delay(POT_DELAY);

  digitalWrite(CSn_PIN, HIGH);
  delay(POT_DELAY);
}

