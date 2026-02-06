// I06 Challenge 3: Stoppuhr
// Stoppuhr mit Start/Stop/Reset Tastern
// Verwendet millis() fuer praezises Timing
// RTC wird fuer Echtzeit-Anzeige parallel genutzt

#include <Wire.h>
#include <RTClib.h>

// RTC-Objekt
RTC_DS1307 rtc;

// Pin-Definitionen
const int BTN_START_PIN = 2;    // Start/Stop Taster
const int BTN_RESET_PIN = 3;    // Reset Taster
const int LED_RUN_PIN = 8;      // LED zeigt laufende Stoppuhr

// Stoppuhr-Variablen
unsigned long startZeit = 0;
unsigned long gestoppteZeit = 0;
unsigned long zwischenzeit = 0;
bool laeuft = false;

// Debounce
unsigned long lastDebounce = 0;
const int DEBOUNCE_DELAY = 200;

void setup() {
    Serial.begin(9600);

    // Pin-Modi setzen
    pinMode(BTN_START_PIN, INPUT_PULLUP);
    pinMode(BTN_RESET_PIN, INPUT_PULLUP);
    pinMode(LED_RUN_PIN, OUTPUT);

    // RTC initialisieren
    if (!rtc.begin()) {
        Serial.println("RTC nicht gefunden!");
        while (true);
    }

    if (!rtc.isrunning()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    Serial.println("=== Stoppuhr ===");
    Serial.println("Taster 1: Start/Stop");
    Serial.println("Taster 2: Reset");
    Serial.println("----------------");
    zeigeZeit(0);
}

void loop() {
    // Tasten abfragen
    if (millis() - lastDebounce > DEBOUNCE_DELAY) {
        // Start/Stop Taster
        if (digitalRead(BTN_START_PIN) == LOW) {
            if (laeuft) {
                // Stoppen
                gestoppteZeit += millis() - startZeit;
                laeuft = false;
                Serial.println("[STOP]");
            } else {
                // Starten
                startZeit = millis();
                laeuft = true;
                Serial.println("[START]");
            }
            lastDebounce = millis();
        }

        // Reset Taster
        if (digitalRead(BTN_RESET_PIN) == LOW) {
            gestoppteZeit = 0;
            startZeit = millis();
            if (!laeuft) {
                zeigeZeit(0);
            }
            Serial.println("[RESET]");
            lastDebounce = millis();
        }
    }

    // LED zeigt Status
    digitalWrite(LED_RUN_PIN, laeuft);

    // Zeit berechnen und anzeigen
    unsigned long aktuelleZeit;
    if (laeuft) {
        aktuelleZeit = gestoppteZeit + (millis() - startZeit);
    } else {
        aktuelleZeit = gestoppteZeit;
    }

    // Anzeige aktualisieren (alle 100ms wenn laeuft)
    static unsigned long letzteAnzeige = 0;
    if (laeuft && (millis() - letzteAnzeige >= 100)) {
        zeigeZeit(aktuelleZeit);
        letzteAnzeige = millis();
    }

    // Aktuelle Uhrzeit alle 5 Sekunden anzeigen
    static unsigned long letzteUhrzeit = 0;
    if (millis() - letzteUhrzeit >= 5000) {
        DateTime now = rtc.now();
        Serial.print("(Uhrzeit: ");
        if (now.hour() < 10) Serial.print("0");
        Serial.print(now.hour());
        Serial.print(":");
        if (now.minute() < 10) Serial.print("0");
        Serial.print(now.minute());
        Serial.print(":");
        if (now.second() < 10) Serial.print("0");
        Serial.print(now.second());
        Serial.println(")");
        letzteUhrzeit = millis();
    }

    delay(50);
}

void zeigeZeit(unsigned long ms) {
    // Umrechnung in Minuten:Sekunden:Hundertstel
    unsigned long hundertstel = (ms / 10) % 100;
    unsigned long sekunden = (ms / 1000) % 60;
    unsigned long minuten = (ms / 60000) % 60;
    unsigned long stunden = ms / 3600000;

    Serial.print("Stoppuhr: ");

    if (stunden > 0) {
        if (stunden < 10) Serial.print("0");
        Serial.print(stunden);
        Serial.print(":");
    }

    if (minuten < 10) Serial.print("0");
    Serial.print(minuten);
    Serial.print(":");

    if (sekunden < 10) Serial.print("0");
    Serial.print(sekunden);
    Serial.print(".");

    if (hundertstel < 10) Serial.print("0");
    Serial.println(hundertstel);
}
