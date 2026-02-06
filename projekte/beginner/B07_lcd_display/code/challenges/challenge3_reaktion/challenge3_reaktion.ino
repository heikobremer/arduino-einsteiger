// B07 Challenge 3: Reaktionsspiel
// Druecke den Taster wenn "JETZT!" erscheint

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD: Adresse 0x27, 16 Spalten, 2 Zeilen
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int TASTER_PIN = 2;

// Spielvariablen
unsigned long startZeit = 0;
unsigned long reaktionsZeit = 0;
bool warteAufReaktion = false;
bool spielLaeuft = false;
int runden = 0;
unsigned long gesamtZeit = 0;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(TASTER_PIN, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("Reaktionsspiel!");
  lcd.setCursor(0, 1);
  lcd.print("Taster = Start");

  Serial.begin(9600);
  Serial.println("Reaktionsspiel - Druecke Taster zum Starten!");
}

void loop() {
  bool tasterGedrueckt = (digitalRead(TASTER_PIN) == LOW);

  // Spiel starten oder reagieren
  if (tasterGedrueckt && !spielLaeuft) {
    // Neues Spiel starten
    spielLaeuft = true;
    warteAufReaktion = false;
    runden = 0;
    gesamtZeit = 0;
    neueRunde();
  }
  else if (tasterGedrueckt && warteAufReaktion) {
    // Reaktion gemessen!
    reaktionsZeit = millis() - startZeit;
    gesamtZeit += reaktionsZeit;
    runden++;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Zeit: ");
    lcd.print(reaktionsZeit);
    lcd.print(" ms");

    Serial.print("Runde ");
    Serial.print(runden);
    Serial.print(": ");
    Serial.print(reaktionsZeit);
    Serial.println(" ms");

    // Bewertung anzeigen
    lcd.setCursor(0, 1);
    if (reaktionsZeit < 200) {
      lcd.print("BLITZSCHNELL!");
    } else if (reaktionsZeit < 350) {
      lcd.print("Sehr gut!");
    } else if (reaktionsZeit < 500) {
      lcd.print("Gut!");
    } else {
      lcd.print("Uebe weiter...");
    }

    warteAufReaktion = false;

    delay(2000);

    // Nach 5 Runden: Ergebnis anzeigen
    if (runden >= 5) {
      zeigeErgebnis();
    } else {
      neueRunde();
    }
  }
  else if (tasterGedrueckt && spielLaeuft && !warteAufReaktion) {
    // Zu frueh gedrueckt!
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ZU FRUEH!");
    lcd.setCursor(0, 1);
    lcd.print("Warte auf JETZT!");

    Serial.println("Zu frueh gedrueckt!");

    delay(1500);
    neueRunde();
  }

  // Entprellen
  if (tasterGedrueckt) {
    delay(50);
    while (digitalRead(TASTER_PIN) == LOW);
    delay(50);
  }
}

void neueRunde() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Runde ");
  lcd.print(runden + 1);
  lcd.print("/5");
  lcd.setCursor(0, 1);
  lcd.print("Warte...");

  // Zufaellige Wartezeit (2-5 Sekunden)
  int warteZeit = random(2000, 5000);
  delay(warteZeit);

  // JETZT!
  lcd.setCursor(0, 1);
  lcd.print(">>> JETZT! <<<");
  startZeit = millis();
  warteAufReaktion = true;

  Serial.println("JETZT!");
}

void zeigeErgebnis() {
  unsigned long durchschnitt = gesamtZeit / 5;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Durchschnitt:");
  lcd.setCursor(0, 1);
  lcd.print(durchschnitt);
  lcd.print(" ms ");

  if (durchschnitt < 250) {
    lcd.print("TOP!");
  } else if (durchschnitt < 400) {
    lcd.print("GUT!");
  }

  Serial.print("Durchschnitt: ");
  Serial.print(durchschnitt);
  Serial.println(" ms");

  spielLaeuft = false;

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reaktionsspiel!");
  lcd.setCursor(0, 1);
  lcd.print("Taster = Start");
}
