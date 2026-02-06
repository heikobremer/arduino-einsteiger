// I06 Challenge 1: LCD Uhr
// Zeigt Uhrzeit und Datum auf einem 16x2 LCD Display
// Verwendet RTC DS1307 und LCD mit I2C-Adapter

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

// RTC-Objekt
RTC_DS1307 rtc;

// LCD mit I2C-Adresse 0x27 (16 Zeichen, 2 Zeilen)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Wochentage abgekuerzt (3 Zeichen)
const char* wochentage[] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};

void setup() {
    Serial.begin(9600);

    // LCD initialisieren
    lcd.init();
    lcd.backlight();
    lcd.clear();

    // RTC initialisieren
    if (!rtc.begin()) {
        lcd.print("RTC Fehler!");
        Serial.println("RTC nicht gefunden!");
        while (true);
    }

    // Zeit setzen falls RTC nicht laeuft
    if (!rtc.isrunning()) {
        Serial.println("RTC laeuft nicht - setze Zeit...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    lcd.print("RTC LCD Uhr");
    delay(1500);
    lcd.clear();
}

void loop() {
    DateTime now = rtc.now();

    // Zeile 1: Zeit im Format HH:MM:SS
    lcd.setCursor(0, 0);
    if (now.hour() < 10) lcd.print("0");
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10) lcd.print("0");
    lcd.print(now.minute());
    lcd.print(":");
    if (now.second() < 10) lcd.print("0");
    lcd.print(now.second());
    lcd.print("   ");  // Alte Zeichen loeschen

    // Zeile 2: Datum im Format DD.MM.YY Wochentag
    lcd.setCursor(0, 1);
    if (now.day() < 10) lcd.print("0");
    lcd.print(now.day());
    lcd.print(".");
    if (now.month() < 10) lcd.print("0");
    lcd.print(now.month());
    lcd.print(".");
    lcd.print(now.year() % 100);  // Nur letzte 2 Stellen
    lcd.print(" ");
    lcd.print(wochentage[now.dayOfTheWeek()]);
    lcd.print("   ");

    // Auch auf Serial ausgeben
    Serial.print("Zeit: ");
    if (now.hour() < 10) Serial.print("0");
    Serial.print(now.hour());
    Serial.print(":");
    if (now.minute() < 10) Serial.print("0");
    Serial.print(now.minute());
    Serial.print(":");
    if (now.second() < 10) Serial.print("0");
    Serial.println(now.second());

    delay(500);  // 2x pro Sekunde aktualisieren
}
