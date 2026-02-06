// B02 Challenge 4: Countdown
// Zeigt im Serial Monitor die Sekunden bis zum naechsten Wechsel

const int LED_ROT = 7;
const int LED_GELB = 6;
const int LED_GRUEN = 5;

void ampel(int rot, int gelb, int gruen) {
  digitalWrite(LED_ROT, rot);
  digitalWrite(LED_GELB, gelb);
  digitalWrite(LED_GRUEN, gruen);
}

void countdown(int sekunden, const char* phase) {
  for (int i = sekunden; i > 0; i--) {
    Serial.print(phase);
    Serial.print(" - noch ");
    Serial.print(i);
    Serial.println(" Sekunde(n)");
    delay(1000);
  }
}

void setup() {
  pinMode(LED_ROT, OUTPUT);
  pinMode(LED_GELB, OUTPUT);
  pinMode(LED_GRUEN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Ampel mit Countdown gestartet!");
}

void loop() {
  // Rot - 3 Sekunden
  ampel(HIGH, LOW, LOW);
  countdown(3, "ROT");

  // Rot + Gelb - 1 Sekunde
  ampel(HIGH, HIGH, LOW);
  countdown(1, "ROT+GELB");

  // Gruen - 3 Sekunden
  ampel(LOW, LOW, HIGH);
  countdown(3, "GRUEN");

  // Gelb - 1 Sekunde
  ampel(LOW, HIGH, LOW);
  countdown(1, "GELB");

  Serial.println("--- Zyklus komplett ---");
}
