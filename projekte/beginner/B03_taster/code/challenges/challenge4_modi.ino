// B03 Challenge 4: Blink-Modi wechseln
// Taster wechselt zwischen verschiedenen LED-Effekten

const int LED_PIN = 7;
const int TASTER_PIN = 6;

int modus = 0;
const int ANZAHL_MODI = 4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TASTER_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Blink-Modi - Druecke zum Wechseln!");
  zeigeModus();
}

void zeigeModus() {
  Serial.print("Modus ");
  Serial.print(modus);
  Serial.print(": ");
  switch (modus) {
    case 0: Serial.println("Langsam (1 Hz)"); break;
    case 1: Serial.println("Schnell (4 Hz)"); break;
    case 2: Serial.println("Herzschlag"); break;
    case 3: Serial.println("SOS"); break;
  }
}

void loop() {
  // Taster pruefen
  if (digitalRead(TASTER_PIN) == LOW) {
    delay(50);
    if (digitalRead(TASTER_PIN) == LOW) {
      modus = (modus + 1) % ANZAHL_MODI;
      zeigeModus();

      // Warten bis losgelassen
      while (digitalRead(TASTER_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }

  // Aktuellen Modus ausfuehren
  switch (modus) {
    case 0:  // Langsam
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
      break;

    case 1:  // Schnell
      digitalWrite(LED_PIN, HIGH);
      delay(125);
      digitalWrite(LED_PIN, LOW);
      delay(125);
      break;

    case 2:  // Herzschlag
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(600);
      break;

    case 3:  // SOS
      // S (...)
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(150);
        digitalWrite(LED_PIN, LOW);
        delay(150);
      }
      delay(300);
      // O (---)
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(400);
        digitalWrite(LED_PIN, LOW);
        delay(150);
      }
      delay(300);
      // S (...)
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(150);
        digitalWrite(LED_PIN, LOW);
        delay(150);
      }
      delay(1000);
      break;
  }
}
