// A07: Light Theremin
// Lichtgesteuertes Musikinstrument mit LDR, Piezo, Poti und LED

// Pin-Definitionen
const int LDR_PIN = A0;       // Fotowiderstand (Licht -> Tonhöhe)
const int POTI_PIN = A1;      // Potentiometer (Tonbereich einstellen)
const int PIEZO_PIN = 9;      // Piezo-Summer (PWM)
const int LED_PIN = 8;        // LED (zeigt Aktivität)

// Tonbereich
const int FREQ_MIN = 100;     // Tiefster Ton (Hz)
const int FREQ_MAX = 2000;    // Höchster Ton (Hz)

// Kalibrierung
int ldrMin = 1023;            // Dunkelster gemessener Wert
int ldrMax = 0;               // Hellster gemessener Wert
bool kalibriert = false;

// Glättung
const int SMOOTH_COUNT = 5;
int ldrValues[SMOOTH_COUNT];
int ldrIndex = 0;

// LED Blink
unsigned long lastBlink = 0;
bool ledState = false;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);

  // Array initialisieren
  for (int i = 0; i < SMOOTH_COUNT; i++) {
    ldrValues[i] = 512;
  }

  Serial.println(F("========================"));
  Serial.println(F("   LIGHT THEREMIN"));
  Serial.println(F("========================"));
  Serial.println(F("Bewege deine Hand über"));
  Serial.println(F("den Lichtsensor!"));
  Serial.println(F(""));
  Serial.println(F("Poti: Tonbereich"));
  Serial.println(F("========================"));
  Serial.println(F("Kalibriere... bewege Hand!"));

  // Kurze Kalibrierungsphase
  calibrate();
}

void calibrate() {
  digitalWrite(LED_PIN, HIGH);

  unsigned long startTime = millis();
  while (millis() - startTime < 3000) {  // 3 Sekunden kalibrieren
    int ldr = analogRead(LDR_PIN);

    if (ldr < ldrMin) ldrMin = ldr;
    if (ldr > ldrMax) ldrMax = ldr;

    // Fortschritt anzeigen
    if ((millis() - startTime) % 500 < 50) {
      Serial.print(F("."));
    }
    delay(50);
  }

  // Sicherheitsbereich hinzufügen
  int range = ldrMax - ldrMin;
  if (range < 50) {
    // Falls Bereich zu klein, Standardwerte verwenden
    ldrMin = 0;
    ldrMax = 1023;
  }

  kalibriert = true;
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.print(F("LDR Bereich: "));
  Serial.print(ldrMin);
  Serial.print(F(" - "));
  Serial.println(ldrMax);
  Serial.println(F("Bereit! Spiel los!"));
  Serial.println();

  // Bereit-Signal
  tone(PIEZO_PIN, 440, 100);
  delay(150);
  tone(PIEZO_PIN, 880, 100);
  delay(200);
}

int getSmoothedLDR() {
  // Neuen Wert hinzufügen
  ldrValues[ldrIndex] = analogRead(LDR_PIN);
  ldrIndex = (ldrIndex + 1) % SMOOTH_COUNT;

  // Durchschnitt berechnen
  long sum = 0;
  for (int i = 0; i < SMOOTH_COUNT; i++) {
    sum += ldrValues[i];
  }
  return sum / SMOOTH_COUNT;
}

void loop() {
  // LDR-Wert lesen (geglättet)
  int ldrValue = getSmoothedLDR();

  // Poti lesen für Tonbereich-Skalierung
  int potiValue = analogRead(POTI_PIN);
  int maxFreq = map(potiValue, 0, 1023, 500, FREQ_MAX);

  // LDR-Wert in Frequenz umwandeln
  int freq = map(ldrValue, ldrMin, ldrMax, FREQ_MIN, maxFreq);
  freq = constrain(freq, FREQ_MIN, maxFreq);

  // Ton erzeugen
  tone(PIEZO_PIN, freq);

  // LED blinken lassen basierend auf Frequenz
  unsigned long blinkInterval = map(freq, FREQ_MIN, maxFreq, 200, 20);
  unsigned long currentTime = millis();

  if (currentTime - lastBlink >= blinkInterval) {
    lastBlink = currentTime;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }

  // Debug-Ausgabe (nicht zu oft)
  static unsigned long lastPrint = 0;
  if (currentTime - lastPrint >= 250) {
    lastPrint = currentTime;

    Serial.print(F("LDR: "));
    Serial.print(ldrValue);
    Serial.print(F(" | Freq: "));
    Serial.print(freq);
    Serial.print(F(" Hz | Max: "));
    Serial.print(maxFreq);
    Serial.println(F(" Hz"));
  }

  delay(10);  // Kurze Pause für Stabilität
}
