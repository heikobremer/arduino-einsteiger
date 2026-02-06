// I06 Challenge 2: Wecker mit Alarm
// Wecker mit einstellbarer Zeit und Buzzer
// Taster 1: Stunden einstellen
// Taster 2: Minuten einstellen
// Taster 3: Alarm an/aus

#include <Wire.h>
#include <RTClib.h>

// RTC-Objekt
RTC_DS1307 rtc;

// Pin-Definitionen
const int BUZZER_PIN = 9;       // Buzzer mit PWM
const int BTN_HOUR_PIN = 2;     // Taster fuer Stunden
const int BTN_MIN_PIN = 3;      // Taster fuer Minuten
const int BTN_ALARM_PIN = 4;    // Taster fuer Alarm an/aus
const int LED_PIN = 8;          // LED zeigt Alarm-Status

// Alarm-Einstellungen
int alarmStunde = 7;    // Standard: 07:00
int alarmMinute = 0;
bool alarmAktiv = false;
bool alarmAusgeloest = false;

// Debounce-Variablen
unsigned long lastDebounce = 0;
const int DEBOUNCE_DELAY = 200;

void setup() {
    Serial.begin(9600);

    // Pin-Modi setzen
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BTN_HOUR_PIN, INPUT_PULLUP);
    pinMode(BTN_MIN_PIN, INPUT_PULLUP);
    pinMode(BTN_ALARM_PIN, INPUT_PULLUP);

    // RTC initialisieren
    if (!rtc.begin()) {
        Serial.println("RTC nicht gefunden!");
        while (true);
    }

    if (!rtc.isrunning()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    Serial.println("=== RTC Wecker ===");
    Serial.println("Taster 1: Stunde +1");
    Serial.println("Taster 2: Minute +1");
    Serial.println("Taster 3: Alarm an/aus");
    zeigeAlarmzeit();
}

void loop() {
    DateTime now = rtc.now();

    // Tasten abfragen mit Debounce
    if (millis() - lastDebounce > DEBOUNCE_DELAY) {
        // Stunden erhoehen
        if (digitalRead(BTN_HOUR_PIN) == LOW) {
            alarmStunde = (alarmStunde + 1) % 24;
            alarmAusgeloest = false;  // Reset bei Aenderung
            zeigeAlarmzeit();
            lastDebounce = millis();
        }

        // Minuten erhoehen
        if (digitalRead(BTN_MIN_PIN) == LOW) {
            alarmMinute = (alarmMinute + 1) % 60;
            alarmAusgeloest = false;
            zeigeAlarmzeit();
            lastDebounce = millis();
        }

        // Alarm an/aus schalten
        if (digitalRead(BTN_ALARM_PIN) == LOW) {
            alarmAktiv = !alarmAktiv;
            alarmAusgeloest = false;

            // Buzzer stoppen falls Alarm aus
            if (!alarmAktiv) {
                noTone(BUZZER_PIN);
            }

            Serial.print("Alarm: ");
            Serial.println(alarmAktiv ? "AN" : "AUS");
            zeigeAlarmzeit();
            lastDebounce = millis();
        }
    }

    // LED zeigt Alarm-Status
    digitalWrite(LED_PIN, alarmAktiv);

    // Alarm pruefen
    if (alarmAktiv && !alarmAusgeloest) {
        if (now.hour() == alarmStunde && now.minute() == alarmMinute) {
            alarmAusgeloest = true;
            Serial.println("!!! ALARM !!!");
        }
    }

    // Alarm-Ton ausgeben (30 Sekunden lang)
    if (alarmAusgeloest && now.second() < 30) {
        // Wechselnder Ton fuer Aufmerksamkeit
        if ((now.second() % 2) == 0) {
            tone(BUZZER_PIN, 1000);  // 1 kHz
        } else {
            tone(BUZZER_PIN, 1500);  // 1.5 kHz
        }
    } else if (alarmAusgeloest) {
        noTone(BUZZER_PIN);
        alarmAusgeloest = false;  // Alarm nach 30 Sek aus
    }

    // Zeit ausgeben (jede Sekunde)
    static int letzteSekunde = -1;
    if (now.second() != letzteSekunde) {
        letzteSekunde = now.second();

        Serial.print("Zeit: ");
        if (now.hour() < 10) Serial.print("0");
        Serial.print(now.hour());
        Serial.print(":");
        if (now.minute() < 10) Serial.print("0");
        Serial.print(now.minute());
        Serial.print(":");
        if (now.second() < 10) Serial.print("0");
        Serial.println(now.second());
    }

    delay(100);
}

void zeigeAlarmzeit() {
    Serial.print("Alarmzeit: ");
    if (alarmStunde < 10) Serial.print("0");
    Serial.print(alarmStunde);
    Serial.print(":");
    if (alarmMinute < 10) Serial.print("0");
    Serial.print(alarmMinute);
    Serial.print(" [");
    Serial.print(alarmAktiv ? "AN" : "AUS");
    Serial.println("]");
}
