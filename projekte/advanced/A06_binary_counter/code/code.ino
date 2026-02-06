// A06: Binary Counter
// 8-Bit Binärzähler (0-255) mit 8 LEDs und 3 Buttons

// Pin-Definitionen für 8 LEDs (Bit 0-7)
const int LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};

// Buttons
const int BTN_UP = 10;      // Count Up
const int BTN_DOWN = 11;    // Count Down
const int BTN_RESET = 12;   // Reset

// Zählerwert (0-255)
byte counter = 0;

// Entprellung
unsigned long lastButtonTime = 0;
const unsigned long DEBOUNCE_DELAY = 150;

// Auto-Count Modus
bool autoMode = false;
unsigned long lastAutoTime = 0;
const unsigned long AUTO_INTERVAL = 500;  // 0.5s pro Schritt

void setup() {
  Serial.begin(9600);

  // LEDs als Output
  for (int i = 0; i < 8; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  // Buttons als Input mit Pull-up
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  Serial.println(F("========================"));
  Serial.println(F("   8-BIT BINARY COUNTER"));
  Serial.println(F("========================"));
  Serial.println(F("Taster:"));
  Serial.println(F("  UP     - Hochzählen"));
  Serial.println(F("  DOWN   - Runterzählen"));
  Serial.println(F("  RESET  - Auf 0 setzen"));
  Serial.println(F("  UP+DOWN lange - Auto-Modus"));
  Serial.println(F("========================"));

  // Startanimation
  startAnimation();

  displayValue();
}

void startAnimation() {
  // LEDs nacheinander einschalten (Knight Rider Style)
  for (int i = 0; i < 8; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(50);
  }
  delay(200);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(LED_PINS[i], LOW);
    delay(50);
  }
}

void displayValue() {
  // Binärwert auf LEDs anzeigen
  for (int i = 0; i < 8; i++) {
    // Bit i des Zählers prüfen
    bool bitValue = (counter >> i) & 1;
    digitalWrite(LED_PINS[i], bitValue ? HIGH : LOW);
  }

  // Serial Ausgabe
  Serial.print(F("Dezimal: "));
  if (counter < 100) Serial.print(F(" "));
  if (counter < 10) Serial.print(F(" "));
  Serial.print(counter);

  Serial.print(F("  Binär: "));
  for (int i = 7; i >= 0; i--) {
    Serial.print((counter >> i) & 1);
    if (i == 4) Serial.print(F(" "));  // Nibble-Trenner
  }

  Serial.print(F("  Hex: 0x"));
  if (counter < 16) Serial.print(F("0"));
  Serial.println(counter, HEX);
}

void countUp() {
  if (counter < 255) {
    counter++;
  } else {
    counter = 0;  // Überlauf: zurück auf 0
    Serial.println(F(">>> Überlauf! <<<"));
  }
  displayValue();
}

void countDown() {
  if (counter > 0) {
    counter--;
  } else {
    counter = 255;  // Unterlauf: auf Maximum
    Serial.println(F(">>> Unterlauf! <<<"));
  }
  displayValue();
}

void resetCounter() {
  counter = 0;
  Serial.println(F(">>> Reset auf 0 <<<"));
  displayValue();

  // Kurze Animation
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 8; j++) {
      digitalWrite(LED_PINS[j], HIGH);
    }
    delay(100);
    for (int j = 0; j < 8; j++) {
      digitalWrite(LED_PINS[j], LOW);
    }
    delay(100);
  }
  displayValue();
}

void checkButtons() {
  unsigned long currentTime = millis();

  // Entprellung
  if (currentTime - lastButtonTime < DEBOUNCE_DELAY) {
    return;
  }

  // Up-Button
  if (digitalRead(BTN_UP) == LOW) {
    lastButtonTime = currentTime;
    countUp();
    delay(50);  // Zusätzliche Entprellung
  }

  // Down-Button
  if (digitalRead(BTN_DOWN) == LOW) {
    lastButtonTime = currentTime;
    countDown();
    delay(50);
  }

  // Reset-Button
  if (digitalRead(BTN_RESET) == LOW) {
    lastButtonTime = currentTime;
    resetCounter();
    delay(200);
  }

  // Auto-Modus: Beide Buttons gleichzeitig lange drücken
  if (digitalRead(BTN_UP) == LOW && digitalRead(BTN_DOWN) == LOW) {
    delay(1000);  // 1 Sekunde halten
    if (digitalRead(BTN_UP) == LOW && digitalRead(BTN_DOWN) == LOW) {
      autoMode = !autoMode;
      Serial.print(F("Auto-Modus: "));
      Serial.println(autoMode ? F("AN") : F("AUS"));

      // Feedback: alle LEDs blinken
      for (int i = 0; i < 8; i++) {
        digitalWrite(LED_PINS[i], HIGH);
      }
      delay(300);
      displayValue();

      lastButtonTime = currentTime;
      delay(500);
    }
  }
}

void loop() {
  checkButtons();

  // Auto-Modus: Automatisch hochzählen
  if (autoMode) {
    unsigned long currentTime = millis();
    if (currentTime - lastAutoTime >= AUTO_INTERVAL) {
      lastAutoTime = currentTime;
      countUp();
    }
  }
}
