// B03 Challenge 1: Langer Druck erkennen
// Kurz (<500ms) vs Lang (>1 Sekunde) unterscheiden

const int LED_PIN = 7;
const int TASTER_PIN = 6;

unsigned long drueckStart = 0;
bool tasterGedrueckt = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TASTER_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Druecke kurz oder lang!");
}

void loop() {
  int tasterStatus = digitalRead(TASTER_PIN);

  if (tasterStatus == LOW && !tasterGedrueckt) {
    // Taster wurde gerade gedrueckt
    drueckStart = millis();
    tasterGedrueckt = true;
    digitalWrite(LED_PIN, HIGH);  // LED an waehrend gedrueckt
  }

  if (tasterStatus == HIGH && tasterGedrueckt) {
    // Taster wurde losgelassen
    unsigned long drueckDauer = millis() - drueckStart;
    tasterGedrueckt = false;
    digitalWrite(LED_PIN, LOW);

    if (drueckDauer < 500) {
      Serial.print("KURZ gedrueckt (");
      Serial.print(drueckDauer);
      Serial.println(" ms)");
      // Kurzes Blinken
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
      }
    } else if (drueckDauer > 1000) {
      Serial.print("LANG gedrueckt (");
      Serial.print(drueckDauer);
      Serial.println(" ms)");
      // Langes Leuchten
      digitalWrite(LED_PIN, HIGH);
      delay(2000);
      digitalWrite(LED_PIN, LOW);
    } else {
      Serial.print("Mittel gedrueckt (");
      Serial.print(drueckDauer);
      Serial.println(" ms)");
    }

    delay(50);  // Entprellen
  }
}
