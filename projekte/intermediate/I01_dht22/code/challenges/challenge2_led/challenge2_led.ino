// Challenge 2: Temperatur-LED-Anzeige
// Rote LED bei >25°C, Blaue LED bei <18°C

#include <DHT.h>

// DHT22 an Pin 7
#define DHT_PIN 7
#define DHT_TYPE DHT22

// LED Pins
#define LED_ROT 8    // Rote LED fuer Waerme (>25°C)
#define LED_BLAU 9   // Blaue LED fuer Kaelte (<18°C)

// Temperaturschwellen
#define TEMP_WARM 25.0
#define TEMP_KALT 18.0

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();

    // LED Pins als Ausgang
    pinMode(LED_ROT, OUTPUT);
    pinMode(LED_BLAU, OUTPUT);

    // LEDs initial aus
    digitalWrite(LED_ROT, LOW);
    digitalWrite(LED_BLAU, LOW);

    Serial.println("DHT22 Temperatur-LED-Anzeige");
    Serial.println("============================");
    Serial.print("Rot: >");
    Serial.print(TEMP_WARM, 0);
    Serial.print("C | Blau: <");
    Serial.print(TEMP_KALT, 0);
    Serial.println("C");
    Serial.println();
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

    // LEDs steuern basierend auf Temperatur
    if (temp > TEMP_WARM) {
        // Zu warm - rote LED an
        digitalWrite(LED_ROT, HIGH);
        digitalWrite(LED_BLAU, LOW);
        Serial.print("[ROT] ");
    } else if (temp < TEMP_KALT) {
        // Zu kalt - blaue LED an
        digitalWrite(LED_ROT, LOW);
        digitalWrite(LED_BLAU, HIGH);
        Serial.print("[BLAU] ");
    } else {
        // Angenehm - beide LEDs aus
        digitalWrite(LED_ROT, LOW);
        digitalWrite(LED_BLAU, LOW);
        Serial.print("[OK] ");
    }

    // Ausgabe
    Serial.print("Temperatur: ");
    Serial.print(temp, 1);
    Serial.print(" C | Feuchtigkeit: ");
    Serial.print(hum, 1);
    Serial.println(" %");
}
