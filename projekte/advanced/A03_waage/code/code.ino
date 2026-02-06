// A03: Digitale Waage mit HX711
// Präzisionsmessung mit Wägezelle

#include "HX711.h"

// Pin-Definitionen
const int HX711_DT = 4;       // HX711 Data Pin
const int HX711_SCK = 5;      // HX711 Clock Pin
const int BTN_TARE = 2;       // Taster für Tara
const int BTN_UNIT = 3;       // Taster für Einheitenwechsel

// HX711 Objekt
HX711 scale;

// Konfiguration
const float KALIBRIERUNG = 420.0;  // Anpassen mit bekanntem Gewicht!
const float STABIL_TOLERANZ = 0.5; // Gramm Toleranz für Stabilität

// Einheiten
const char* EINHEITEN[] = {"g", "kg", "oz"};
const float FAKTOREN[] = {1.0, 0.001, 0.035274};
const int ANZAHL_EINHEITEN = 3;
int einheitIndex = 0;

// Stabilitätserkennung
float letzteWerte[5];
int werteIndex = 0;
bool werteGefuellt = false;

// Entprellung
unsigned long lastButtonTime = 0;
const unsigned long DEBOUNCE_DELAY = 200;

void setup() {
  Serial.begin(9600);

  // Taster mit Pull-up
  pinMode(BTN_TARE, INPUT_PULLUP);
  pinMode(BTN_UNIT, INPUT_PULLUP);

  // HX711 initialisieren
  Serial.println(F("Digitale Waage mit HX711"));
  Serial.println(F("========================"));
  Serial.println(F("Initialisiere..."));

  scale.begin(HX711_DT, HX711_SCK);
  scale.set_scale(KALIBRIERUNG);

  delay(1000);

  Serial.println(F("Tara setzen..."));
  scale.tare(15);  // 15 Messungen für Tara

  Serial.println(F("Waage bereit!"));
  Serial.println(F("Taster 1: Tara | Taster 2: Einheit"));
  Serial.println(F("========================"));
}

bool istStabil() {
  if (!werteGefuellt) return false;

  float minVal = letzteWerte[0];
  float maxVal = letzteWerte[0];

  for (int i = 1; i < 5; i++) {
    if (letzteWerte[i] < minVal) minVal = letzteWerte[i];
    if (letzteWerte[i] > maxVal) maxVal = letzteWerte[i];
  }

  return (maxVal - minVal) < STABIL_TOLERANZ;
}

void loop() {
  unsigned long currentTime = millis();

  // Taster prüfen (mit Entprellung)
  if (currentTime - lastButtonTime > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_TARE) == LOW) {
      Serial.println(F("Setze Tara..."));
      scale.tare(15);
      Serial.println(F("Tara gesetzt!"));
      lastButtonTime = currentTime;
      delay(300);
    }

    if (digitalRead(BTN_UNIT) == LOW) {
      einheitIndex = (einheitIndex + 1) % ANZAHL_EINHEITEN;
      Serial.print(F("Einheit: "));
      Serial.println(EINHEITEN[einheitIndex]);
      lastButtonTime = currentTime;
      delay(300);
    }
  }

  // Gewicht messen (Mittelwert aus 3 Messungen)
  float gewichtGramm = scale.get_units(3);

  // Stabilitätsprüfung
  letzteWerte[werteIndex] = gewichtGramm;
  werteIndex = (werteIndex + 1) % 5;
  if (werteIndex == 0) werteGefuellt = true;

  bool stabil = istStabil();

  // Umrechnung in aktuelle Einheit
  float anzeige = gewichtGramm * FAKTOREN[einheitIndex];

  // Ausgabe
  Serial.print(F("Gewicht: "));

  if (einheitIndex == 1) {  // kg
    Serial.print(anzeige, 3);
  } else {
    Serial.print(anzeige, 1);
  }

  Serial.print(F(" "));
  Serial.print(EINHEITEN[einheitIndex]);

  if (stabil) {
    Serial.println(F(" [STABIL]"));
  } else {
    Serial.println();
  }

  delay(200);
}
