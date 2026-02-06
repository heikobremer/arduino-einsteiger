// B07 Challenge 4: Menue-Navigation
// Mit 2 Tastern durch ein Menue blaettern (Hoch/Runter)

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD: Adresse 0x27, 16 Spalten, 2 Zeilen
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int TASTER_HOCH = 2;
const int TASTER_RUNTER = 3;

// Menue-Eintraege
const char* menuePunkte[] = {
  "1. LED an/aus",
  "2. Helligkeit",
  "3. Blinkmodus",
  "4. Geschwindigk.",
  "5. Info anzeigen",
  "6. Reset"
};
const int ANZAHL_MENUE = 6;

int aktuellerPunkt = 0;
int letzterPunkt = -1;  // Fuer Aktualisierungspruefung

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(TASTER_HOCH, INPUT_PULLUP);
  pinMode(TASTER_RUNTER, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("=== MENUE ===");

  Serial.begin(9600);
  Serial.println("Menue-Demo gestartet!");
  Serial.println("Taster HOCH = Pin 2");
  Serial.println("Taster RUNTER = Pin 3");

  delay(1500);
  zeigeMenu();
}

void loop() {
  // Taster abfragen
  if (digitalRead(TASTER_HOCH) == LOW) {
    aktuellerPunkt--;
    if (aktuellerPunkt < 0) {
      aktuellerPunkt = ANZAHL_MENUE - 1;  // Zu Ende springen
    }

    Serial.print("HOCH -> Punkt ");
    Serial.println(aktuellerPunkt + 1);

    zeigeMenu();
    warteAufLoslassen();
  }

  if (digitalRead(TASTER_RUNTER) == LOW) {
    aktuellerPunkt++;
    if (aktuellerPunkt >= ANZAHL_MENUE) {
      aktuellerPunkt = 0;  // Zu Anfang springen
    }

    Serial.print("RUNTER -> Punkt ");
    Serial.println(aktuellerPunkt + 1);

    zeigeMenu();
    warteAufLoslassen();
  }

  delay(50);  // Entprellen
}

void zeigeMenu() {
  lcd.clear();

  // Zeile 1: Aktueller Menue-Punkt mit Pfeil
  lcd.setCursor(0, 0);
  lcd.print(">");
  lcd.print(menuePunkte[aktuellerPunkt]);

  // Zeile 2: Naechster Punkt (Vorschau)
  int naechsterPunkt = (aktuellerPunkt + 1) % ANZAHL_MENUE;
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print(menuePunkte[naechsterPunkt]);

  // Position-Anzeige rechts
  lcd.setCursor(14, 0);
  lcd.print(aktuellerPunkt + 1);
  lcd.print("/");
  lcd.setCursor(14, 1);
  lcd.print(ANZAHL_MENUE);
}

void warteAufLoslassen() {
  // Warten bis beide Taster losgelassen sind
  while (digitalRead(TASTER_HOCH) == LOW || digitalRead(TASTER_RUNTER) == LOW) {
    delay(10);
  }
  delay(100);  // Extra Entprellzeit
}
