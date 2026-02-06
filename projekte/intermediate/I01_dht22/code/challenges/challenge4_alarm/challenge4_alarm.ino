// Challenge 4: Feuchtigkeitsalarm
// Buzzer ertönt bei Luftfeuchtigkeit >80%

#include <DHT.h>

// DHT22 an Pin 7
#define DHT_PIN 7
#define DHT_TYPE DHT22

// Buzzer an Pin 10
#define BUZZER_PIN 10

// Alarm-Schwelle
#define HUM_ALARM 80.0

DHT dht(DHT_PIN, DHT_TYPE);

bool alarmAktiv = false;

void setup() {
    Serial.begin(9600);
    dht.begin();

    // Buzzer Pin als Ausgang
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println("DHT22 Feuchtigkeitsalarm");
    Serial.println("========================");
    Serial.print("Alarm bei Feuchtigkeit > ");
    Serial.print(HUM_ALARM, 0);
    Serial.println(" %");
    Serial.println();
}

// Alarm-Ton erzeugen (kurze Piepser)
void alarmTon() {
    // 3 kurze Piepser
    for (int i = 0; i < 3; i++) {
        tone(BUZZER_PIN, 2000, 100);  // 2kHz fuer 100ms
        delay(200);
    }
}

void loop() {
    // DHT22 braucht min. 2 Sekunden zwischen Messungen
    delay(2000);

    // Messwerte lesen
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    // Pruefen ob Messung erfolgreich
    if (isnan(temp) || isnan(hum)) {
        Serial.println("Fehler beim Lesen des DHT22!");
        return;
    }

    // Alarm-Logik
    if (hum > HUM_ALARM) {
        if (!alarmAktiv) {
            Serial.println("!!! ALARM: Feuchtigkeit zu hoch !!!");
            alarmAktiv = true;
        }
        // Alarm-Ton abspielen
        alarmTon();
        Serial.print("[ALARM] ");
    } else {
        if (alarmAktiv) {
            Serial.println("--- Alarm beendet ---");
            alarmAktiv = false;
        }
        // Buzzer sicher aus
        noTone(BUZZER_PIN);
        Serial.print("[OK] ");
    }

    // Ausgabe
    Serial.print("Temperatur: ");
    Serial.print(temp, 1);
    Serial.print(" C | Feuchtigkeit: ");
    Serial.print(hum, 1);
    Serial.println(" %");
}
