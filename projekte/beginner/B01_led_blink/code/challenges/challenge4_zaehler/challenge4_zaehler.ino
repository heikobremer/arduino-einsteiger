// B01 Challenge 4: Zähler
// LED blinkt genau 10 Mal und stoppt dann

const int LED_PIN = 7;
const int ANZAHL_BLINKS = 10;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Challenge 4: Zähler - 10x blinken!");

  // Blinken in setup() - läuft nur einmal
  for (int i = 1; i <= ANZAHL_BLINKS; i++) {
    Serial.print("Blink ");
    Serial.println(i);

    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }

  Serial.println("Fertig! LED bleibt jetzt aus.");
}

void loop() {
  // Nichts tun - Programm ist beendet
}
