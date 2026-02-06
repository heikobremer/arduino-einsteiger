// A02: Automatische Pflanzenbewässerung
// Bodenfeuchtesensor + Relais + Pumpensteuerung

// Pin-Definitionen
const int SOIL_PIN = A0;      // Kapazitiver Bodenfeuchtesensor
const int RELAY_PIN = 7;      // Relais für Wasserpumpe
const int LED_STATUS = 8;     // Status-LED (Pumpe aktiv)
const int LED_DRY = 9;        // Warnung: zu trocken (rot)
const int LED_OK = 10;        // Status: Feuchtigkeit OK (grün)

// Konfiguration
// Bei kapazitivem Sensor: hoher Wert = feucht, niedriger Wert = trocken
const int SCHWELLE_TROCKEN = 400;   // Unter diesem Wert -> bewässern
const int SCHWELLE_FEUCHT = 600;    // Über diesem Wert -> stoppen (Hysterese)
const unsigned long MIN_PUMP_ZEIT = 2000;   // Mindest-Pumpzeit in ms
const unsigned long MAX_PUMP_ZEIT = 30000;  // Maximale Pumpzeit (Sicherheit)
const unsigned long CHECK_INTERVAL = 5000;  // Prüfintervall in ms

// Status-Variablen
bool pumpeAktiv = false;
unsigned long pumpStartTime = 0;
unsigned long lastCheck = 0;

void setup() {
  Serial.begin(9600);

  // Pins konfigurieren
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_DRY, OUTPUT);
  pinMode(LED_OK, OUTPUT);

  // Alles ausschalten
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_STATUS, LOW);
  digitalWrite(LED_DRY, LOW);
  digitalWrite(LED_OK, LOW);

  Serial.println(F("Automatische Pflanzenbewässerung"));
  Serial.println(F("================================"));
  Serial.print(F("Schwelle trocken: "));
  Serial.println(SCHWELLE_TROCKEN);
  Serial.print(F("Schwelle feucht:  "));
  Serial.println(SCHWELLE_FEUCHT);
  Serial.print(F("Max. Pumpzeit:    "));
  Serial.print(MAX_PUMP_ZEIT / 1000);
  Serial.println(F("s"));
  Serial.println(F("================================"));
}

int readMoisture() {
  // Mehrere Messungen für stabilen Wert
  long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(SOIL_PIN);
    delay(10);
  }
  return sum / 5;
}

void updateLEDs(int rawValue) {
  if (rawValue < SCHWELLE_TROCKEN) {
    digitalWrite(LED_DRY, HIGH);
    digitalWrite(LED_OK, LOW);
  } else if (rawValue > SCHWELLE_FEUCHT) {
    digitalWrite(LED_DRY, LOW);
    digitalWrite(LED_OK, HIGH);
  } else {
    digitalWrite(LED_DRY, LOW);
    digitalWrite(LED_OK, LOW);
  }
}

void startPump() {
  if (!pumpeAktiv) {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_STATUS, HIGH);
    pumpeAktiv = true;
    pumpStartTime = millis();
    Serial.println(F(">>> PUMPE EIN <<<"));
  }
}

void stopPump() {
  if (pumpeAktiv) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_STATUS, LOW);
    pumpeAktiv = false;
    Serial.println(F(">>> PUMPE AUS <<<"));
  }
}

bool checkPumpTimeout() {
  if (pumpeAktiv) {
    unsigned long laufzeit = millis() - pumpStartTime;
    if (laufzeit >= MAX_PUMP_ZEIT) {
      Serial.print(F("!!! SICHERHEIT: Max. Pumpzeit erreicht ("));
      Serial.print(MAX_PUMP_ZEIT / 1000);
      Serial.println(F("s)"));
      stopPump();
      return true;
    }
  }
  return false;
}

void loop() {
  unsigned long currentTime = millis();

  // Nur alle CHECK_INTERVAL ms prüfen
  if (currentTime - lastCheck < CHECK_INTERVAL) {
    return;
  }
  lastCheck = currentTime;

  // Feuchtigkeit messen
  int rawValue = readMoisture();
  int prozent = map(rawValue, 0, 1023, 0, 100);

  // LEDs aktualisieren
  updateLEDs(rawValue);

  // Status ausgeben
  Serial.print(F("Feuchtigkeit: "));
  Serial.print(prozent);
  Serial.print(F("% (raw: "));
  Serial.print(rawValue);
  Serial.print(F(") | "));

  if (rawValue < SCHWELLE_TROCKEN) {
    Serial.print(F("TROCKEN"));
  } else if (rawValue > SCHWELLE_FEUCHT) {
    Serial.print(F("FEUCHT"));
  } else {
    Serial.print(F("OK"));
  }

  Serial.print(F(" | "));
  Serial.println(pumpeAktiv ? F("PUMPT") : F("IDLE"));

  // Sicherheitsprüfung
  if (checkPumpTimeout()) {
    delay(60000);  // Nach Sicherheitsabschaltung warten
    return;
  }

  // Bewässerungslogik mit Hysterese
  if (rawValue < SCHWELLE_TROCKEN && !pumpeAktiv) {
    Serial.println(F("Boden zu trocken - starte Bewässerung"));
    startPump();
  } else if (rawValue > SCHWELLE_FEUCHT && pumpeAktiv) {
    // Mindestpumpzeit beachten
    unsigned long laufzeit = millis() - pumpStartTime;
    if (laufzeit >= MIN_PUMP_ZEIT) {
      Serial.print(F("Boden feucht genug - stoppe nach "));
      Serial.print(laufzeit / 1000.0, 1);
      Serial.println(F("s"));
      stopPump();
    }
  }
}
