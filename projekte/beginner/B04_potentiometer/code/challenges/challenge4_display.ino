// B04 Challenge 4: Prozent auf LCD anzeigen
// LCD 1602 I2C zeigt den Poti-Wert in Prozent

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int POTI_PIN = A0;

// LCD: Adresse 0x27, 16 Spalten, 2 Zeilen
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Poti-Anzeige");

  Serial.begin(9600);
  Serial.println("Poti auf LCD - Drehe am Poti!");
}

void loop() {
  int wert = analogRead(POTI_PIN);  // 0-1023
  int prozent = wert * 100 / 1023;

  // Zeile 2: Prozent und Balken
  lcd.setCursor(0, 1);
  lcd.print(prozent);
  lcd.print("%  ");

  // Balkenanzeige (10 Zeichen)
  lcd.setCursor(6, 1);
  int balken = prozent / 10;  // 0-10
  for (int i = 0; i < 10; i++) {
    if (i < balken) {
      lcd.print((char)255);  // Voller Block
    } else {
      lcd.print("-");
    }
  }

  Serial.print("Wert: ");
  Serial.print(wert);
  Serial.print(" = ");
  Serial.print(prozent);
  Serial.println("%");

  delay(100);
}
