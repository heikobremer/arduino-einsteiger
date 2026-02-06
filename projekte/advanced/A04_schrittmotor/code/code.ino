// A04: Schrittmotor 28BYJ-48 mit ULN2003
// Verschiedene Schrittmodi und Positionierung

#include <Stepper.h>

// Pin-Definitionen für ULN2003
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

// Potentiometer und Taster
const int POTI_PIN = A0;
const int BTN_RICHTUNG = 2;

// 28BYJ-48 Spezifikationen:
// Schritte pro Umdrehung: 2048 (bei Stepper-Library im Full-Step-Modus)
const int SCHRITTE_PRO_UMDREHUNG = 2048;

// Stepper-Objekt erstellen
Stepper motor(SCHRITTE_PRO_UMDREHUNG, IN1, IN3, IN2, IN4);

// Variablen
int richtung = 1;  // 1 = CW, -1 = CCW
unsigned long lastButtonTime = 0;
const unsigned long DEBOUNCE_DELAY = 300;

// Halbschritt-Sequenz für manuelle Steuerung
const int HALBSCHRITT[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};
int schrittIndex = 0;

void setup() {
  Serial.begin(9600);

  // Pins konfigurieren
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(BTN_RICHTUNG, INPUT_PULLUP);

  Serial.println(F("Schrittmotor 28BYJ-48 Steuerung"));
  Serial.println(F("==============================="));
  Serial.println(F("Poti: Geschwindigkeit"));
  Serial.println(F("Taster: Richtungswechsel"));
  Serial.println(F("==============================="));

  // Demo starten
  demo();

  Serial.println(F("\nInteraktiver Modus - Poti dreht Motor"));
}

void setStep(int step[4]) {
  digitalWrite(IN1, step[0]);
  digitalWrite(IN2, step[1]);
  digitalWrite(IN3, step[2]);
  digitalWrite(IN4, step[3]);
}

void halbSchritt(int schritte, int delayMs) {
  int dir = (schritte > 0) ? 1 : -1;
  schritte = abs(schritte);

  for (int i = 0; i < schritte; i++) {
    schrittIndex = (schrittIndex + dir + 8) % 8;
    setStep((int*)HALBSCHRITT[schrittIndex]);
    delay(delayMs);
  }
}

void motorAus() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void demo() {
  Serial.println(F("\n--- DEMO START ---"));

  // Demo 1: Verschiedene Geschwindigkeiten mit Stepper-Library
  Serial.println(F("Demo 1: Geschwindigkeiten (Library)"));

  int geschwindigkeiten[] = {5, 10, 15};
  for (int i = 0; i < 3; i++) {
    Serial.print(F("  "));
    Serial.print(geschwindigkeiten[i]);
    Serial.println(F(" RPM..."));
    motor.setSpeed(geschwindigkeiten[i]);
    motor.step(SCHRITTE_PRO_UMDREHUNG / 4);  // 90 Grad
    delay(500);
  }

  // Demo 2: Halbschritt-Modus manuell
  Serial.println(F("\nDemo 2: Halbschritt (manuell)"));
  Serial.println(F("  Halbschritt vorwärts..."));
  halbSchritt(512, 2);  // 45 Grad
  delay(500);

  Serial.println(F("  Halbschritt rückwärts..."));
  halbSchritt(-512, 2);
  delay(500);

  // Motor ausschalten
  motorAus();

  Serial.println(F("\n--- DEMO ENDE ---"));
}

void loop() {
  unsigned long currentTime = millis();

  // Taster für Richtungswechsel
  if (currentTime - lastButtonTime > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_RICHTUNG) == LOW) {
      richtung *= -1;
      Serial.print(F("Richtung: "));
      Serial.println(richtung > 0 ? F("CW") : F("CCW"));
      lastButtonTime = currentTime;
    }
  }

  // Poti lesen für Geschwindigkeit (0-1023 -> 1-15 RPM)
  int potiWert = analogRead(POTI_PIN);
  int rpm = map(potiWert, 0, 1023, 1, 15);

  // Geschwindigkeit setzen und einen kleinen Schritt machen
  motor.setSpeed(rpm);
  motor.step(richtung);  // Ein Schritt in gewählte Richtung
}
