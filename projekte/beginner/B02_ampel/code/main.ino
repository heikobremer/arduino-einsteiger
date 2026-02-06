// B02: Ampelschaltung - Fertige Version
// Drei LEDs in deutscher Ampelsequenz

const int LED_ROT = 7;    // Rote LED an Pin 7
const int LED_GELB = 6;   // Gelbe LED an Pin 6
const int LED_GRUEN = 5;  // Gruene LED an Pin 5

void setup() {
  // Alle LED-Pins als Ausgang konfigurieren
  pinMode(LED_ROT, OUTPUT);
  pinMode(LED_GELB, OUTPUT);
  pinMode(LED_GRUEN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Ampelschaltung gestartet!");
}

// Hilfsfunktion: Setzt den Zustand aller drei LEDs
void ampel(int rot, int gelb, int gruen) {
  digitalWrite(LED_ROT, rot);
  digitalWrite(LED_GELB, gelb);
  digitalWrite(LED_GRUEN, gruen);
}

void loop() {
  // Rot (3 Sekunden)
  ampel(HIGH, LOW, LOW);
  delay(3000);

  // Rot + Gelb (1 Sekunde)
  ampel(HIGH, HIGH, LOW);
  delay(1000);

  // Gruen (3 Sekunden)
  ampel(LOW, LOW, HIGH);
  delay(3000);

  // Gelb (1 Sekunde)
  ampel(LOW, HIGH, LOW);
  delay(1000);
}
