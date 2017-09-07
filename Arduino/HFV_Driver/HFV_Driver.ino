#define BTN_MAX_RES     3
#define BTN_MIN_RES     4
#define BTN_STEP_UP     5
#define BTN_STEP_DOWN   6
#define BTN_NO_CHANGE   0

#define V_READ_PIN      A1
#define POT_PWR_PIN     13

#define CS_N            7
#define UD_CTRL         8
#define INC_N           9

#define NUM_PLACES      100
#define STD_DELAY       15

#define BTN_CHECK_INTERVAL    500 // ms
#define V_READ_INTERVAL       1000

int state = BTN_NO_CHANGE;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(BTN_MAX_RES, INPUT);
  pinMode(BTN_MIN_RES, INPUT);
  pinMode(BTN_STEP_UP, INPUT);
  pinMode(BTN_STEP_DOWN, INPUT);


  pinMode(CS_N, OUTPUT);
  pinMode(UD_CTRL, OUTPUT);
  pinMode(INC_N, OUTPUT);
  pinMode(POT_PWR_PIN, OUTPUT);

  digitalWrite(INC_N, LOW);
  digitalWrite(POT_PWR_PIN, HIGH);
  delay(100);

  Serial.println("HFV Controller online.");
}

void loop() {
  unsigned long currentTime = millis();
  
  updateButton(currentTime);
  checkVoltageIn(currentTime);
}

void checkVoltageIn(unsigned long currentTime) {
  static unsigned long lastVRead = 0;

  if (currentTime - lastVRead > V_READ_INTERVAL) {
    int sensorValue = analogRead(V_READ_PIN);
    Serial.print("\tV: ");
    Serial.println(sensorValue);
    lastVRead = currentTime;
  }
}

void updateButton(unsigned long currentTime) {
  static int placeCtr = 0;
  static unsigned long lastBtnCheck = 0;

  if (currentTime - lastBtnCheck > BTN_CHECK_INTERVAL) {
    if (digitalRead(BTN_MAX_RES) == HIGH) {
      state = BTN_MAX_RES;
      Serial.println("Setting to maximum resistance");
      digitalWrite(CS_N, LOW);
      digitalWrite(UD_CTRL, HIGH);
      delay(STD_DELAY);
    } else if (digitalRead(BTN_MIN_RES) == HIGH) {
      state = BTN_MIN_RES;
      Serial.println("Setting to minimum resistance");
      digitalWrite(CS_N, LOW);
      digitalWrite(UD_CTRL, LOW);
      delay(STD_DELAY);
    } else if (digitalRead(BTN_STEP_UP) == HIGH) {
      state = BTN_STEP_UP;
      Serial.println("Stepping up one level");
      digitalWrite(CS_N, LOW);
      digitalWrite(UD_CTRL, HIGH);
      delay(STD_DELAY);
    } else if (digitalRead(BTN_STEP_DOWN) == HIGH) {
      state = BTN_STEP_DOWN;
      Serial.println("Stepping down one level");
      digitalWrite(CS_N, LOW);
      digitalWrite(UD_CTRL, LOW);
      delay(STD_DELAY);
    } 

    lastBtnCheck = currentTime;
  }

  switch (state) {
  case BTN_MAX_RES:
  case BTN_MIN_RES:
    digitalWrite(INC_N, HIGH);
    delay(STD_DELAY);
    digitalWrite(INC_N, LOW);
    delay(STD_DELAY);

    if (placeCtr++ >= NUM_PLACES) {
      placeCtr = 0;
      state = BTN_NO_CHANGE;
    }
    
    break;
    
  case BTN_STEP_UP:
  case BTN_STEP_DOWN:
    digitalWrite(INC_N, HIGH);
    delay(STD_DELAY);
    digitalWrite(INC_N, LOW);
    delay(STD_DELAY);
    state = BTN_NO_CHANGE;
    
    break;
  default:
    digitalWrite(CS_N, HIGH);
    delay(STD_DELAY);
  }
}

