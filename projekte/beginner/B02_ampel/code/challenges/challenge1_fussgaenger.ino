// B02 Challenge 1: Fußgängerampel
// Taster drücken -> Ampel wechselt auf Grün für Fußgänger

const int LED_ROT = 7;
const int LED_GELB = 6;
const int LED_GRUEN = 5;
const int TASTER_PIN = 4;

void ampel(int rot, int gelb, int gruen) {
  digitalWrite(LED_ROT, rot);
  digitalWrite(LED_GELB, gelb);
  digitalWrite(LED_GRUEN, gruen);
}

void setup() {
  pinMode(LED_ROT, OUTPUT);
  pinMode(LED_GELB, OUTPUT);
  pinMode(LED_GRUEN, OUTPUT);
  pinMode(TASTER_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Fussgaengerampel - Druecke den Taster!");
}

void loop() {
  // Normal: Rot fuer Fussgaenger (Auto hat Gruen)
  ampel(LOW, LOW, HIGH);

  // Warten auf Tastendruck
  if (digitalRead(TASTER_PIN) == LOW) {
    delay(50);  // Entprellen
    if (digitalRead(TASTER_PIN) == LOW) {
      Serial.println("Taster gedrueckt - Fussgaenger kommt!");

      // Autos: Gelb
      ampel(LOW, HIGH, LOW);
      delay(2000);

      // Autos: Rot (Fussgaenger hat Gruen)
      ampel(HIGH, LOW, LOW);
      Serial.println("Fussgaenger kann gehen!");
      delay(5000);

      // Autos: Rot+Gelb
      ampel(HIGH, HIGH, LOW);
      delay(1000);

      // Zurueck zu Gruen fuer Autos
      Serial.println("Autos fahren wieder");
    }
  }
  delay(100);
}
