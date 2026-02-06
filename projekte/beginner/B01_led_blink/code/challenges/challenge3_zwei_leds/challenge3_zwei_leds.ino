// B01 Challenge 3: Zwei LEDs abwechselnd
// LED1 an Pin 7, LED2 an Pin 8

const int LED1_PIN = 7;
const int LED2_PIN = 8;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Challenge 3: Zwei LEDs abwechselnd!");
}

void loop() {
  // LED1 an, LED2 aus
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);
  delay(500);

  // LED1 aus, LED2 an
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);
  delay(500);
}
