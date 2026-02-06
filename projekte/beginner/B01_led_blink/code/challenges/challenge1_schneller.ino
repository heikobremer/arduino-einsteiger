// B01 Challenge 1: Schneller blinken
// LED blinkt mit 100ms statt 500ms

const int LED_PIN = 7;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Challenge 1: Schnelles Blinken!");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(100);  // Nur 100ms statt 500ms

  digitalWrite(LED_PIN, LOW);
  delay(100);
}
