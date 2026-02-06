// Challenge 3: LCD-Anzeige fuer DHT22
// Zeigt Temperatur und Feuchtigkeit auf LCD1602 mit I2C

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DHT22 an Pin 7
#define DHT_PIN 7
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

// LCD mit I2C Adresse 0x27, 16 Zeichen, 2 Zeilen
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Custom Character: Grad-Symbol
byte gradSymbol[] = {
    B00110,
    B01001,
    B01001,
    B00110,
    B00000,
    B00000,
    B00000,
    B00000
};

void setup() {
    Serial.begin(9600);
    dht.begin();

    // LCD initialisieren
    lcd.init();
    lcd.backlight();

    // Custom Character erstellen
    lcd.createChar(0, gradSymbol);

    // Startbildschirm
    lcd.setCursor(0, 0);
    lcd.print("DHT22 Sensor");
    lcd.setCursor(0, 1);
    lcd.print("Initialisiere...");

    Serial.println("DHT22 LCD-Anzeige");
    Serial.println("=================");

    delay(2000);
    lcd.clear();
}

void loop() {
    // DHT22 braucht min. 2 Sekunden zwischen Messungen
    delay(2000);

    // Messwerte lesen
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    // Pruefen ob Messung erfolgreich
    if (isnan(temp) || isnan(hum)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sensor Fehler!");
        Serial.println("Fehler beim Lesen des DHT22!");
        return;
    }

    // LCD Zeile 1: Temperatur
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp, 1);
    lcd.write(0);  // Grad-Symbol
    lcd.print("C    ");

    // LCD Zeile 2: Feuchtigkeit
    lcd.setCursor(0, 1);
    lcd.print("Hum:  ");
    lcd.print(hum, 1);
    lcd.print(" %   ");

    // Serielle Ausgabe
    Serial.print("Temperatur: ");
    Serial.print(temp, 1);
    Serial.print(" C | Feuchtigkeit: ");
    Serial.print(hum, 1);
    Serial.println(" %");
}
