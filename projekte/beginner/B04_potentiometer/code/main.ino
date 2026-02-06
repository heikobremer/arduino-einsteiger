// B04: Potentiometer - Fertige Version
// Poti steuert LED-Helligkeit ueber PWM

const int POTI_PIN = A0;  // Potentiometer an A0
const int LED_PIN = 9;    // LED mit PWM an Pin 9

void setup() {
  // LED-Pin als Ausgang (PWM-faehig)
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Drehe am Potentiometer!");
}

void loop() {
  // Potentiometer-Wert lesen (0-1023)
  int wert = analogRead(POTI_PIN);

  // Wert auf PWM-Bereich mappen (0-255)
  int helligkeit = map(wert, 0, 1023, 0, 255);

  // LED mit PWM dimmen
  analogWrite(LED_PIN, helligkeit);

  // Prozent berechnen fuer Anzeige
  int prozent = wert * 100 / 1023;

  Serial.print("Helligkeit: ");
  Serial.print(prozent);
  Serial.println("%");

  delay(100);
}
