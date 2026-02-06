// B07 Challenge 2: Temperatur-Anzeige mit Potentiometer
// Poti-Wert wird als simulierte Temperatur (0-40°C) angezeigt

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD: Adresse 0x27, 16 Spalten, 2 Zeilen
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int POTI_PIN = A0;

// Temperatur-Bereich
const int TEMP_MIN = 0;
const int TEMP_MAX = 40;

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Temperatur-Sim.");

  Serial.begin(9600);
  Serial.println("Drehe am Poti fuer Temperatur!");
}

void loop() {
  // Poti-Wert lesen (0-1023)
  int potiWert = analogRead(POTI_PIN);

  // Auf Temperatur-Bereich umrechnen (0-40°C)
  float temperatur = map(potiWert, 0, 1023, TEMP_MIN * 10, TEMP_MAX * 10) / 10.0;

  // Temperatur anzeigen
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperatur, 1);  // Eine Dezimalstelle
  lcd.print((char)223);      // Grad-Symbol
  lcd.print("C   ");

  // Status-Anzeige basierend auf Temperatur
  lcd.setCursor(13, 1);
  if (temperatur < 10) {
    lcd.print("BR!");  // Brr - kalt
  } else if (temperatur < 25) {
    lcd.print("OK ");  // Angenehm
  } else if (temperatur < 35) {
    lcd.print("WRM");  // Warm
  } else {
    lcd.print("HOT");  // Heiss!
  }

  // Debug-Ausgabe
  Serial.print("Poti: ");
  Serial.print(potiWert);
  Serial.print(" -> Temperatur: ");
  Serial.print(temperatur);
  Serial.println(" C");

  delay(200);
}
