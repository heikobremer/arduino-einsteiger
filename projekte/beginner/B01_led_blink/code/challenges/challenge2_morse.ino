// B01 Challenge 2: Morse-Code SOS
// Drei kurze, drei lange, drei kurze Signale

const int LED_PIN = 7;
const int KURZ = 200;    // Kurzes Signal (dit)
const int LANG = 600;    // Langes Signal (dah)
const int PAUSE = 200;   // Pause zwischen Signalen
const int BUCHSTABEN_PAUSE = 600;  // Pause zwischen Buchstaben

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Challenge 2: Morse-Code SOS!");
}

void blink(int dauer) {
  digitalWrite(LED_PIN, HIGH);
  delay(dauer);
  digitalWrite(LED_PIN, LOW);
  delay(PAUSE);
}

void loop() {
  // S = ...  (drei kurze)
  blink(KURZ);
  blink(KURZ);
  blink(KURZ);
  delay(BUCHSTABEN_PAUSE);

  // O = ---  (drei lange)
  blink(LANG);
  blink(LANG);
  blink(LANG);
  delay(BUCHSTABEN_PAUSE);

  // S = ...  (drei kurze)
  blink(KURZ);
  blink(KURZ);
  blink(KURZ);

  // Lange Pause vor Wiederholung
  delay(2000);
}
