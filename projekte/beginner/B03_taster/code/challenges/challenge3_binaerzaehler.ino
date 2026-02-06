// B03 Challenge 3: Binärzähler mit 3 LEDs
// Zeigt den Zählerstand binär an (0-7)

const int LED_1 = 7;  // Bit 0 (Wert 1)
const int LED_2 = 8;  // Bit 1 (Wert 2)
const int LED_3 = 9;  // Bit 2 (Wert 4)
const int TASTER_PIN = 6;

int zaehler = 0;

void zeigeBinaer(int wert) {
  // Bit 0 (1er Stelle)
  digitalWrite(LED_1, (wert & 1) ? HIGH : LOW);
  // Bit 1 (2er Stelle)
  digitalWrite(LED_2, (wert & 2) ? HIGH : LOW);
  // Bit 2 (4er Stelle)
  digitalWrite(LED_3, (wert & 4) ? HIGH : LOW);

  Serial.print("Dezimal: ");
  Serial.print(wert);
  Serial.print(" -> Binaer: ");
  Serial.print((wert & 4) ? 1 : 0);
  Serial.print((wert & 2) ? 1 : 0);
  Serial.println((wert & 1) ? 1 : 0);
}

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(TASTER_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Binaerzaehler 0-7");
  Serial.println("Druecke den Taster zum Hochzaehlen!");

  zeigeBinaer(zaehler);
}

void loop() {
  if (digitalRead(TASTER_PIN) == LOW) {
    delay(50);  // Entprellen
    if (digitalRead(TASTER_PIN) == LOW) {
      zaehler++;
      if (zaehler > 7) {
        zaehler = 0;  // Ueberlauf
        Serial.println("--- Ueberlauf! Zurueck auf 0 ---");
      }
      zeigeBinaer(zaehler);

      // Warten bis losgelassen
      while (digitalRead(TASTER_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
}
