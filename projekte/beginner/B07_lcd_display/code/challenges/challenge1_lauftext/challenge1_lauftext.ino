// B07 Challenge 1: Scrollender Lauftext
// Text bewegt sich von rechts nach links ueber das Display

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD: Adresse 0x27, 16 Spalten, 2 Zeilen
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Der anzuzeigende Text
const char* nachricht = "Arduino macht Spass!   ";
int textLaenge;
int position = 16;  // Startet rechts ausserhalb des Displays

void setup() {
  lcd.init();
  lcd.backlight();

  textLaenge = strlen(nachricht);

  lcd.setCursor(0, 0);
  lcd.print("=== Lauftext ===");

  Serial.begin(9600);
  Serial.println("Lauftext gestartet!");
}

void loop() {
  // Zeile 2 loeschen
  lcd.setCursor(0, 1);
  lcd.print("                ");  // 16 Leerzeichen

  // Text an aktueller Position anzeigen
  for (int i = 0; i < 16; i++) {
    int zeichenIndex = i - position;

    // Nur anzeigen wenn innerhalb des Textes
    if (zeichenIndex >= 0 && zeichenIndex < textLaenge) {
      lcd.setCursor(i, 1);
      lcd.print(nachricht[zeichenIndex]);
    }
  }

  // Position nach links verschieben
  position--;

  // Wenn Text komplett durchgelaufen, neu starten
  if (position < -textLaenge) {
    position = 16;
    Serial.println("Text neu gestartet");
  }

  delay(250);  // Scroll-Geschwindigkeit
}
