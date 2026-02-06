// B05: RGB-LED Farbmischer - Fertige Version
// Drei Potis steuern die RGB-Kanaele

const int POTI_R = A0;  // Potentiometer fuer Rot
const int POTI_G = A1;  // Potentiometer fuer Gruen
const int POTI_B = A2;  // Potentiometer fuer Blau

const int LED_R = 9;    // Rote LED an PWM-Pin 9
const int LED_G = 10;   // Gruene LED an PWM-Pin 10
const int LED_B = 11;   // Blaue LED an PWM-Pin 11

void setup() {
  // RGB-LED Pins als Ausgang
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  Serial.begin(9600);
  Serial.println("RGB-Farbmischer gestartet!");
  Serial.println("Drehe an den Potis um Farben zu mischen.");
}

void loop() {
  // Potis lesen (0-1023) und auf PWM mappen (0-255)
  int r = map(analogRead(POTI_R), 0, 1023, 0, 255);
  int g = map(analogRead(POTI_G), 0, 1023, 0, 255);
  int b = map(analogRead(POTI_B), 0, 1023, 0, 255);

  // RGB-LED steuern
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);

  // RGB-Werte anzeigen
  Serial.print("R:");
  Serial.print(r);
  Serial.print(" G:");
  Serial.print(g);
  Serial.print(" B:");
  Serial.println(b);

  delay(100);
}
