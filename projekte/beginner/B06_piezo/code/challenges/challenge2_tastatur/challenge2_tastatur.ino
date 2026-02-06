// B06 Challenge 2: Mini-Tastatur
// 4 Taster spielen 4 verschiedene Toene (C, D, E, F)

const int BUZZER_PIN = 9;  // Piezo an PWM-Pin 9

// Taster-Pins
const int TASTER_C = 2;
const int TASTER_D = 3;
const int TASTER_E = 4;
const int TASTER_F = 5;

// Noten-Frequenzen
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  // Taster mit internem Pull-Up
  pinMode(TASTER_C, INPUT_PULLUP);
  pinMode(TASTER_D, INPUT_PULLUP);
  pinMode(TASTER_E, INPUT_PULLUP);
  pinMode(TASTER_F, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Mini-Tastatur bereit!");
  Serial.println("Druecke Taster fuer C, D, E, F");
}

void loop() {
  // Taster abfragen (LOW = gedrueckt wegen Pull-Up)
  if (digitalRead(TASTER_C) == LOW) {
    tone(BUZZER_PIN, NOTE_C4);
    Serial.println("Note C");
  }
  else if (digitalRead(TASTER_D) == LOW) {
    tone(BUZZER_PIN, NOTE_D4);
    Serial.println("Note D");
  }
  else if (digitalRead(TASTER_E) == LOW) {
    tone(BUZZER_PIN, NOTE_E4);
    Serial.println("Note E");
  }
  else if (digitalRead(TASTER_F) == LOW) {
    tone(BUZZER_PIN, NOTE_F4);
    Serial.println("Note F");
  }
  else {
    // Kein Taster gedrueckt - Ton aus
    noTone(BUZZER_PIN);
  }

  delay(10);  // Kurze Pause fuer Stabilitaet
}
