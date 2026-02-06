// A08: Mini-Oszilloskop
// Echtzeit-Signaldarstellung über Serial Plotter

// Pin-Definitionen
const int SIGNAL_PIN = A0;     // Analogeingang (Messsignal)
const int ZEITBASIS_PIN = A1;  // Potentiometer für Zeitbasis
const int BTN_TRIGGER = 2;     // Taster für Trigger-Modus
const int BTN_MODUS = 3;       // Taster für Anzeige-Modus
const int TEST_SIGNAL_PIN = 9; // PWM-Testsignal

// Konstanten
const int SAMPLES = 128;       // Anzahl Samples pro Durchgang
const int TRIGGER_TIMEOUT = 100; // ms

// Zeitbasis-Stufen (Mikrosekunden pro Sample)
const int ZEITBASIS_STUFEN[] = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000};
const int ANZAHL_STUFEN = 9;

// Modi
const char* MODI[] = {"NORMAL", "HOLD", "AUTO"};
const int ANZAHL_MODI = 3;

// Variablen
int daten[SAMPLES];
int zeitbasisIndex = 3;  // Start: 100us
int modusIndex = 0;      // Start: NORMAL
bool triggerAktiv = true;
int triggerLevel = 512;  // Mitte (0-1023)
int minWert = 1023;
int maxWert = 0;
float frequenz = 0;

// Entprellung
unsigned long lastButtonTime = 0;
const unsigned long DEBOUNCE_DELAY = 200;

void setup() {
  Serial.begin(115200);  // Hohe Baudrate für schnelle Datenübertragung

  // Pins konfigurieren
  pinMode(BTN_TRIGGER, INPUT_PULLUP);
  pinMode(BTN_MODUS, INPUT_PULLUP);
  pinMode(TEST_SIGNAL_PIN, OUTPUT);

  // PWM-Testsignal generieren (ca. 490 Hz bei Arduino Uno)
  analogWrite(TEST_SIGNAL_PIN, 128);  // 50% Duty Cycle

  Serial.println(F("Mini-Oszilloskop"));
  Serial.println(F("================"));
  Serial.println(F("A0: Signaleingang (0-5V)"));
  Serial.println(F("D9: Testsignal (PWM)"));
  Serial.println(F("D2: Trigger-Modus"));
  Serial.println(F("D3: Anzeige-Modus"));
  Serial.println(F("A1: Zeitbasis (Poti)"));
  Serial.println(F("================"));
  Serial.println(F("Nutze Serial Plotter für Darstellung!"));
  delay(2000);
}

void berechneFrequenz() {
  if (maxWert - minWert < 50) {
    frequenz = 0;
    return;
  }

  int mitte = (minWert + maxWert) / 2;
  int kreuzungen = 0;
  bool letzter = daten[0] > mitte;

  for (int i = 1; i < SAMPLES; i++) {
    bool aktuell = daten[i] > mitte;
    if (aktuell != letzter) {
      kreuzungen++;
    }
    letzter = aktuell;
  }

  if (kreuzungen > 1) {
    float zeitGesamt = (float)SAMPLES * ZEITBASIS_STUFEN[zeitbasisIndex] / 1000000.0;
    frequenz = (kreuzungen / 2.0) / zeitGesamt;
  } else {
    frequenz = 0;
  }
}

bool warteTrigger() {
  unsigned long timeout = millis() + TRIGGER_TIMEOUT;
  int letzterWert = analogRead(SIGNAL_PIN);

  while (millis() < timeout) {
    int aktuellerWert = analogRead(SIGNAL_PIN);

    // Steigende Flanke durch Trigger-Level?
    if (letzterWert < triggerLevel && aktuellerWert >= triggerLevel) {
      return true;
    }

    letzterWert = aktuellerWert;
    delayMicroseconds(10);
  }

  return false;  // Timeout
}

void sampleSignal() {
  int delayUs = ZEITBASIS_STUFEN[zeitbasisIndex];

  // Bei aktivem Trigger warten
  if (triggerAktiv && modusIndex != 2) {  // Nicht bei AUTO
    warteTrigger();
  }

  // Samples aufnehmen
  minWert = 1023;
  maxWert = 0;

  for (int i = 0; i < SAMPLES; i++) {
    int wert = analogRead(SIGNAL_PIN);
    daten[i] = wert;

    if (wert < minWert) minWert = wert;
    if (wert > maxWert) maxWert = wert;

    delayMicroseconds(delayUs);
  }

  berechneFrequenz();
}

void ausgabe() {
  // Für Serial Plotter: Nur die Werte ausgeben
  for (int i = 0; i < SAMPLES; i++) {
    Serial.println(daten[i]);
  }
}

void ausgabeInfo() {
  // Info-Ausgabe (wird vom Plotter ignoriert wenn mit // kommentiert)
  Serial.print(F("// Zeitbasis: "));
  Serial.print(ZEITBASIS_STUFEN[zeitbasisIndex]);
  Serial.print(F("us | Freq: "));

  if (frequenz >= 1000) {
    Serial.print(frequenz / 1000, 1);
    Serial.print(F("kHz"));
  } else {
    Serial.print(frequenz, 0);
    Serial.print(F("Hz"));
  }

  Serial.print(F(" | Modus: "));
  Serial.print(MODI[modusIndex]);
  Serial.print(F(" | Trigger: "));
  Serial.println(triggerAktiv ? F("AN") : F("AUS"));
}

void loop() {
  unsigned long currentTime = millis();

  // Taster prüfen
  if (currentTime - lastButtonTime > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_TRIGGER) == LOW) {
      triggerAktiv = !triggerAktiv;
      Serial.print(F("// Trigger: "));
      Serial.println(triggerAktiv ? F("AN") : F("AUS"));
      lastButtonTime = currentTime;
    }

    if (digitalRead(BTN_MODUS) == LOW) {
      modusIndex = (modusIndex + 1) % ANZAHL_MODI;
      Serial.print(F("// Modus: "));
      Serial.println(MODI[modusIndex]);
      lastButtonTime = currentTime;
    }
  }

  // Zeitbasis über Poti einstellen
  int potiWert = analogRead(ZEITBASIS_PIN);
  int neuerIndex = map(potiWert, 0, 1023, 0, ANZAHL_STUFEN - 1);
  if (neuerIndex != zeitbasisIndex) {
    zeitbasisIndex = neuerIndex;
    Serial.print(F("// Zeitbasis: "));
    Serial.print(ZEITBASIS_STUFEN[zeitbasisIndex]);
    Serial.println(F("us"));
  }

  // HOLD-Modus: Keine neuen Samples
  if (modusIndex != 1) {  // Nicht HOLD
    sampleSignal();
  }

  ausgabe();

  // Info alle 10 Durchgänge
  static int counter = 0;
  if (++counter >= 10) {
    ausgabeInfo();
    counter = 0;
  }

  delay(10);
}
