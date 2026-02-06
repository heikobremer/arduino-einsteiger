// I06 Challenge 4: Countdown-Timer
// Countdown mit einstellbarer Zeit und LED-Warnung
// Taster 1: Minuten +1
// Taster 2: Sekunden +10
// Taster 3: Start/Stop
// LEDs: Gruen (laeuft), Gelb (< 10 Sek), Rot (abgelaufen)

#include <Wire.h>
#include <RTClib.h>

// RTC-Objekt
RTC_DS1307 rtc;

// Pin-Definitionen
const int BTN_MIN_PIN = 2;      // Minuten +1
const int BTN_SEC_PIN = 3;      // Sekunden +10
const int BTN_START_PIN = 4;    // Start/Stop
const int BUZZER_PIN = 9;       // Buzzer
const int LED_GREEN_PIN = 7;    // Gruen: laeuft
const int LED_YELLOW_PIN = 6;   // Gelb: < 10 Sek
const int LED_RED_PIN = 5;      // Rot: abgelaufen

// Countdown-Variablen
int countdownMinuten = 1;       // Standard: 1 Minute
int countdownSekunden = 0;
unsigned long startZeit = 0;
unsigned long gesamtSekunden = 0;
bool laeuft = false;
bool abgelaufen = false;

// Debounce
unsigned long lastDebounce = 0;
const int DEBOUNCE_DELAY = 200;

void setup() {
    Serial.begin(9600);

    // Pin-Modi setzen
    pinMode(BTN_MIN_PIN, INPUT_PULLUP);
    pinMode(BTN_SEC_PIN, INPUT_PULLUP);
    pinMode(BTN_START_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    // RTC initialisieren
    if (!rtc.begin()) {
        Serial.println("RTC nicht gefunden!");
        while (true);
    }

    if (!rtc.isrunning()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    Serial.println("=== Countdown Timer ===");
    Serial.println("Taster 1: Minuten +1");
    Serial.println("Taster 2: Sekunden +10");
    Serial.println("Taster 3: Start/Stop");
    Serial.println("-----------------------");

    berechneGesamtzeit();
    zeigeCountdown();
}

void loop() {
    // Tasten abfragen
    if (millis() - lastDebounce > DEBOUNCE_DELAY) {
        // Minuten erhoehen (nur wenn gestoppt)
        if (digitalRead(BTN_MIN_PIN) == LOW && !laeuft) {
            countdownMinuten = (countdownMinuten + 1) % 60;
            berechneGesamtzeit();
            abgelaufen = false;
            zeigeCountdown();
            lastDebounce = millis();
        }

        // Sekunden erhoehen (nur wenn gestoppt)
        if (digitalRead(BTN_SEC_PIN) == LOW && !laeuft) {
            countdownSekunden = (countdownSekunden + 10) % 60;
            berechneGesamtzeit();
            abgelaufen = false;
            zeigeCountdown();
            lastDebounce = millis();
        }

        // Start/Stop
        if (digitalRead(BTN_START_PIN) == LOW) {
            if (abgelaufen) {
                // Reset nach Ablauf
                abgelaufen = false;
                noTone(BUZZER_PIN);
                berechneGesamtzeit();
                zeigeCountdown();
            } else if (laeuft) {
                // Stoppen und verbleibende Zeit speichern
                unsigned long vergangen = (millis() - startZeit) / 1000;
                if (vergangen < gesamtSekunden) {
                    gesamtSekunden -= vergangen;
                } else {
                    gesamtSekunden = 0;
                }
                laeuft = false;
                Serial.println("[PAUSE]");
            } else if (gesamtSekunden > 0) {
                // Starten
                startZeit = millis();
                laeuft = true;
                Serial.println("[START]");
            }
            lastDebounce = millis();
        }
    }

    // Verbleibende Zeit berechnen
    unsigned long verbleibend = gesamtSekunden;
    if (laeuft) {
        unsigned long vergangen = (millis() - startZeit) / 1000;
        if (vergangen < gesamtSekunden) {
            verbleibend = gesamtSekunden - vergangen;
        } else {
            verbleibend = 0;
            laeuft = false;
            abgelaufen = true;
            Serial.println("!!! ZEIT ABGELAUFEN !!!");
        }
    }

    // LEDs aktualisieren
    aktualisiereLEDs(verbleibend);

    // Alarm bei Ablauf
    if (abgelaufen) {
        // Blinkender Ton
        if ((millis() / 500) % 2 == 0) {
            tone(BUZZER_PIN, 2000);
        } else {
            noTone(BUZZER_PIN);
        }
    }

    // Anzeige aktualisieren
    static unsigned long letzteAnzeige = 0;
    if (millis() - letzteAnzeige >= 500) {
        if (laeuft || abgelaufen) {
            zeigeVerbleibend(verbleibend);
        }
        letzteAnzeige = millis();
    }

    // Aktuelle Uhrzeit von RTC anzeigen (alle 10 Sekunden)
    static unsigned long letzteUhrzeit = 0;
    if (millis() - letzteUhrzeit >= 10000) {
        DateTime now = rtc.now();
        Serial.print("(Uhrzeit: ");
        if (now.hour() < 10) Serial.print("0");
        Serial.print(now.hour());
        Serial.print(":");
        if (now.minute() < 10) Serial.print("0");
        Serial.print(now.minute());
        Serial.println(")");
        letzteUhrzeit = millis();
    }

    delay(50);
}

void berechneGesamtzeit() {
    gesamtSekunden = countdownMinuten * 60 + countdownSekunden;
}

void zeigeCountdown() {
    Serial.print("Eingestellt: ");
    if (countdownMinuten < 10) Serial.print("0");
    Serial.print(countdownMinuten);
    Serial.print(":");
    if (countdownSekunden < 10) Serial.print("0");
    Serial.print(countdownSekunden);
    Serial.print(" (");
    Serial.print(gesamtSekunden);
    Serial.println(" Sek)");
}

void zeigeVerbleibend(unsigned long sek) {
    int min = sek / 60;
    int sec = sek % 60;

    Serial.print("Verbleibend: ");
    if (min < 10) Serial.print("0");
    Serial.print(min);
    Serial.print(":");
    if (sec < 10) Serial.print("0");
    Serial.println(sec);
}

void aktualisiereLEDs(unsigned long verbleibend) {
    // Alle LEDs aus
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);

    if (abgelaufen) {
        // Rot blinkend
        digitalWrite(LED_RED_PIN, (millis() / 250) % 2);
    } else if (laeuft) {
        if (verbleibend <= 10) {
            // Gelb blinkend: weniger als 10 Sekunden
            digitalWrite(LED_YELLOW_PIN, (millis() / 200) % 2);
        } else {
            // Gruen: laeuft normal
            digitalWrite(LED_GREEN_PIN, HIGH);
        }
    }
}
