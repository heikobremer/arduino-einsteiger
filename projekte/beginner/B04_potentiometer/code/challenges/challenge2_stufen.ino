// B04 Challenge 2: Blink-Stufen
// LED blinkt je nach Poti-Position unterschiedlich schnell

const int POTI_PIN = A0;
const int LED_PIN = 9;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Stufen-Blinker - Drehe am Poti!");
}

void loop() {
  int wert = analogRead(POTI_PIN);  // 0-1023
  int stufe = wert / 205;  // 0-4 (5 Stufen)

  int blinkDelay;
  const char* stufenName;

  switch (stufe) {
    case 0:
      blinkDelay = 1000;  // 1 Hz
      stufenName = "Sehr langsam";
      break;
    case 1:
      blinkDelay = 500;   // 2 Hz
      stufenName = "Langsam";
      break;
    case 2:
      blinkDelay = 250;   // 4 Hz
      stufenName = "Mittel";
      break;
    case 3:
      blinkDelay = 100;   // 10 Hz
      stufenName = "Schnell";
      break;
    default:
      blinkDelay = 50;    // 20 Hz
      stufenName = "Sehr schnell";
      break;
  }

  Serial.print("Stufe ");
  Serial.print(stufe);
  Serial.print(": ");
  Serial.println(stufenName);

  digitalWrite(LED_PIN, HIGH);
  delay(blinkDelay);
  digitalWrite(LED_PIN, LOW);
  delay(blinkDelay);
}
