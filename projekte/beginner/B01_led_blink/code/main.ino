// B01: LED Blinken - Fertige Version
// Das "Hello World" der Mikrocontroller

const int LED_PIN = 7;  // LED an Pin 7

void setup() {
  // LED-Pin als Ausgang konfigurieren
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("LED Blink-Programm gestartet!");
}

void loop() {
  // LED einschalten
  digitalWrite(LED_PIN, HIGH);
  delay(500);  // 0.5 Sekunden warten

  // LED ausschalten
  digitalWrite(LED_PIN, LOW);
  delay(500);  // 0.5 Sekunden warten
}
