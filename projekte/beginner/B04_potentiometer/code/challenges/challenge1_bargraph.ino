// B04 Challenge 1: Bargraph (VU-Meter)
// 5 LEDs zeigen den Poti-Wert als Füllstandsanzeige

const int POTI_PIN = A0;

const int LED_1 = 5;
const int LED_2 = 6;
const int LED_3 = 7;
const int LED_4 = 8;
const int LED_5 = 9;

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);

  Serial.begin(9600);
  Serial.println("Bargraph - Drehe am Poti!");
}

void loop() {
  int wert = analogRead(POTI_PIN);  // 0-1023
  int stufe = map(wert, 0, 1023, 0, 5);  // 0-5 LEDs

  // LEDs entsprechend setzen
  digitalWrite(LED_1, stufe >= 1 ? HIGH : LOW);
  digitalWrite(LED_2, stufe >= 2 ? HIGH : LOW);
  digitalWrite(LED_3, stufe >= 3 ? HIGH : LOW);
  digitalWrite(LED_4, stufe >= 4 ? HIGH : LOW);
  digitalWrite(LED_5, stufe >= 5 ? HIGH : LOW);

  // Debug-Ausgabe
  Serial.print("Wert: ");
  Serial.print(wert);
  Serial.print(" -> ");
  for (int i = 0; i < stufe; i++) {
    Serial.print("*");
  }
  Serial.println();

  delay(100);
}
