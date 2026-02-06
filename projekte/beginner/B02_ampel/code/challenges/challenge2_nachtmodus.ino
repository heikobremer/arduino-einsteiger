// B02 Challenge 2: Nachtmodus
// Nur Gelb blinkt (0.5s an, 0.5s aus)

const int LED_ROT = 7;
const int LED_GELB = 6;
const int LED_GRUEN = 5;

void setup() {
  pinMode(LED_ROT, OUTPUT);
  pinMode(LED_GELB, OUTPUT);
  pinMode(LED_GRUEN, OUTPUT);

  // Rot und Gruen dauerhaft aus
  digitalWrite(LED_ROT, LOW);
  digitalWrite(LED_GRUEN, LOW);

  Serial.begin(9600);
  Serial.println("Nachtmodus aktiv - Gelb blinkt!");
}

void loop() {
  digitalWrite(LED_GELB, HIGH);
  delay(500);

  digitalWrite(LED_GELB, LOW);
  delay(500);
}
