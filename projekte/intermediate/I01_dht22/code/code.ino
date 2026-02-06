// I01: DHT22 Temperatur & Feuchtigkeit
// Digitaler Sensor mit eigenem Protokoll

#include <DHT.h>

// DHT22 an Pin 7
#define DHT_PIN 7
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();

    Serial.println("DHT22 Temperatur & Feuchtigkeit");
    Serial.println("------------------------------");
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

    // Ausgabe
    Serial.print("Temperatur: ");
    Serial.print(temp, 1);
    Serial.print(" C | Feuchtigkeit: ");
    Serial.print(hum, 1);
    Serial.println(" %");
}
