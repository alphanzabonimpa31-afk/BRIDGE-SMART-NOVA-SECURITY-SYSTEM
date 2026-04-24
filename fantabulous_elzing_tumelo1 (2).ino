// SMART NOVA SECURITY SYSTEM (Arduino UNO)

// Pins (match your wiring)
const int DOOR_BUTTON = 7;
const int RESET_BUTTON = 8;
const int RED_LED = 2;
const int GREEN_LED = 3;
const int BUZZER = 6;
const int LDR_PIN = A0;

bool alarmActive = false;

// For detecting button press properly
bool lastDoorState = HIGH;
bool lastResetState = HIGH;

void setup() {
  pinMode(DOOR_BUTTON, INPUT_PULLUP);   // IMPORTANT
  pinMode(RESET_BUTTON, INPUT_PULLUP);  // IMPORTANT
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
  Serial.println("MONITORING ACTIVE");

  digitalWrite(GREEN_LED, HIGH);
}

void loop() {

  int doorState = digitalRead(DOOR_BUTTON);
  int resetState = digitalRead(RESET_BUTTON);
  int ldrValue = analogRead(LDR_PIN);

  // ================= NIGHT MODE =================
  if (!alarmActive && ldrValue > 600) {
    Serial.println("NIGHT MODE ON");

    digitalWrite(GREEN_LED, HIGH);
    delay(500);
    digitalWrite(GREEN_LED, LOW);
    delay(500);
  }

  // ================= NORMAL MODE =================
  else if (!alarmActive) {
    digitalWrite(GREEN_LED, HIGH);
  }

  // ================= INTRUDER =================
  if (doorState == LOW && lastDoorState == HIGH && !alarmActive) {
    alarmActive = true;

    Serial.println("INTRUDER DETECTED");

    digitalWrite(GREEN_LED, LOW);

    // Flash 10 times
    for (int i = 0; i < 10; i++) {
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BUZZER, HIGH);
      delay(200);

      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER, LOW);
      delay(200);
    }

    // Stay ON
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
  }

  // ================= RESET =================
  if (resetState == LOW && lastResetState == HIGH && alarmActive) {
    alarmActive = false;

    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(GREEN_LED, HIGH);

    Serial.println("SYSTEM RESET");
    Serial.println("MONITORING ACTIVE");
  }

  // Save states
  lastDoorState = doorState;
  lastResetState = resetState;
}