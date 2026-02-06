// B03: Taster und Entprellung - Fertige Version
// Taster mit Zaehler und LED-Toggle

const int LED_PIN = 7;      // LED an Pin 7
const int TASTER_PIN = 6;   // Taster an Pin 6

int zaehler = 0;
int ledZustand = LOW;

void setup() {
  // LED als Ausgang
  pinMode(LED_PIN, OUTPUT);

  // Taster als Eingang mit internem Pull-up
  pinMode(TASTER_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Druecke den Taster!");
}

void loop() {
  // Taster gedrueckt? (LOW wegen Pull-up)
  if (digitalRead(TASTER_PIN) == LOW) {
    delay(50);  // Entprellzeit abwarten

    // Immer noch gedrueckt?
    if (digitalRead(TASTER_PIN) == LOW) {
      zaehler++;

      // LED umschalten (toggle)
      ledZustand = !ledZustand;
      digitalWrite(LED_PIN, ledZustand);

      Serial.print("Tastendruck #");
      Serial.println(zaehler);

      // Warten bis losgelassen
      while (digitalRead(TASTER_PIN) == LOW) {
        delay(10);
      }
    }
  }
  delay(10);
}
