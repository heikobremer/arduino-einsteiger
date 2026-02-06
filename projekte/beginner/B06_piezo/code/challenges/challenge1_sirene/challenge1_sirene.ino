// B06 Challenge 1: Polizeisirene
// Auf- und absteigende Toene erzeugen eine Sirene

const int BUZZER_PIN = 9;  // Piezo an PWM-Pin 9

// Sirenen-Parameter
const int FREQ_MIN = 400;   // Tiefste Frequenz (Hz)
const int FREQ_MAX = 1200;  // Hoechste Frequenz (Hz)
const int FREQ_STEP = 20;   // Frequenz-Schritte
const int TON_DAUER = 10;   // Dauer pro Ton (ms)

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Polizeisirene gestartet!");
  Serial.println("Reset druecken zum Stoppen");
}

void loop() {
  // Aufsteigende Frequenz
  Serial.println("Aufsteigend...");
  for (int freq = FREQ_MIN; freq <= FREQ_MAX; freq += FREQ_STEP) {
    tone(BUZZER_PIN, freq);
    delay(TON_DAUER);
  }

  // Absteigende Frequenz
  Serial.println("Absteigend...");
  for (int freq = FREQ_MAX; freq >= FREQ_MIN; freq -= FREQ_STEP) {
    tone(BUZZER_PIN, freq);
    delay(TON_DAUER);
  }
}
