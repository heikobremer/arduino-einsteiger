// B03 Challenge 2: Doppelklick erkennen
// Zwei Klicks innerhalb von 300ms = Doppelklick

const int LED_PIN = 7;
const int TASTER_PIN = 6;
const int DOPPELKLICK_ZEIT = 300;  // ms

unsigned long letzterKlick = 0;
int klickZaehler = 0;
bool wartetAufZweiten = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TASTER_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Klicke einfach oder doppelt!");
}

void loop() {
  // Taster pruefen
  if (digitalRead(TASTER_PIN) == LOW) {
    delay(50);  // Entprellen
    if (digitalRead(TASTER_PIN) == LOW) {
      unsigned long jetzt = millis();

      if (wartetAufZweiten && (jetzt - letzterKlick) < DOPPELKLICK_ZEIT) {
        // Doppelklick!
        Serial.println("DOPPELKLICK erkannt!");
        // Schnelles Blinken
        for (int i = 0; i < 5; i++) {
          digitalWrite(LED_PIN, HIGH);
          delay(50);
          digitalWrite(LED_PIN, LOW);
          delay(50);
        }
        wartetAufZweiten = false;
      } else {
        // Erster Klick - auf zweiten warten
        letzterKlick = jetzt;
        wartetAufZweiten = true;
      }

      // Warten bis losgelassen
      while (digitalRead(TASTER_PIN) == LOW) {
        delay(10);
      }
      delay(50);  // Entprellen
    }
  }

  // Timeout fuer Einzelklick
  if (wartetAufZweiten && (millis() - letzterKlick) > DOPPELKLICK_ZEIT) {
    // Einzelklick
    Serial.println("Einzelklick erkannt.");
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    wartetAufZweiten = false;
  }
}
