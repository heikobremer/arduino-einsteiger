// B07: LCD 1602 Display - Fertige Version
// Text auf LCD mit I2C anzeigen

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD-Konfiguration
// Adresse: 0x27 (Standard) oder 0x3F
// 16 Spalten, 2 Zeilen
LiquidCrystal_I2C lcd(0x27, 16, 2);

int zaehler = 0;

void setup() {
  // LCD initialisieren
  lcd.init();
  lcd.backlight();  // Hintergrundbeleuchtung an

  // Startbildschirm
  lcd.setCursor(0, 0);       // Spalte 0, Zeile 0
  lcd.print("Arduino Kurs");
  lcd.setCursor(0, 1);       // Spalte 0, Zeile 1
  lcd.print("LCD mit I2C");

  delay(2000);

  // Zaehler-Demo vorbereiten
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zaehler:");

  Serial.begin(9600);
  Serial.println("LCD gestartet!");
}

void loop() {
  // Zaehler auf LCD anzeigen
  lcd.setCursor(0, 1);
  lcd.print(zaehler);
  lcd.print(" Sek.   ");  // Leerzeichen zum Ueberschreiben

  zaehler++;
  delay(1000);
}
