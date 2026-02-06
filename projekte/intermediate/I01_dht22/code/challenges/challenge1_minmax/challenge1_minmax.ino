// Challenge 1: Min/Max Temperatur und Feuchtigkeit
// Speichert die Extremwerte seit dem Start

#include <DHT.h>

// DHT22 an Pin 7
#define DHT_PIN 7
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

// Variablen fuer Min/Max Werte
float tempMin = 999.0;   // Startwert hoch fuer Minimum
float tempMax = -999.0;  // Startwert niedrig fuer Maximum
float humMin = 999.0;
float humMax = -999.0;

bool ersterMesswert = true;

void setup() {
    Serial.begin(9600);
    dht.begin();

    Serial.println("DHT22 Min/Max Tracker");
    Serial.println("=====================");
    Serial.println("Zeichnet Temperatur- und Feuchtigkeitsextreme auf");
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

    // Min/Max aktualisieren
    if (temp < tempMin) tempMin = temp;
    if (temp > tempMax) tempMax = temp;
    if (hum < humMin) humMin = hum;
    if (hum > humMax) humMax = hum;

    // Aktuelle Werte ausgeben
    Serial.println("--- Aktuelle Messung ---");
    Serial.print("Temperatur: ");
    Serial.print(temp, 1);
    Serial.print(" C | Feuchtigkeit: ");
    Serial.print(hum, 1);
    Serial.println(" %");

    // Min/Max Werte ausgeben
    Serial.println("--- Extremwerte ---");
    Serial.print("Temp Min: ");
    Serial.print(tempMin, 1);
    Serial.print(" C | Temp Max: ");
    Serial.print(tempMax, 1);
    Serial.println(" C");

    Serial.print("Hum Min:  ");
    Serial.print(humMin, 1);
    Serial.print(" % | Hum Max:  ");
    Serial.print(humMax, 1);
    Serial.println(" %");

    Serial.println();
}
