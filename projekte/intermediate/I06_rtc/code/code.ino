// I06: DS1307/DS3231 Echtzeituhr (RTC)
// Uhrzeit und Datum mit Batterie-Backup

#include <Wire.h>
#include <RTClib.h>

// RTC-Objekt (funktioniert mit DS1307 und DS3231)
RTC_DS1307 rtc;

// Wochentage auf Deutsch
const char* wochentage[] = {
    "Sonntag", "Montag", "Dienstag", "Mittwoch",
    "Donnerstag", "Freitag", "Samstag"
};

void setup() {
    Serial.begin(9600);

    // I2C auf A4 (SDA) und A5 (SCL) - Standard Arduino Uno
    if (!rtc.begin()) {
        Serial.println("RTC nicht gefunden!");
        while (true);
    }

    // Pruefen ob RTC laeuft
    if (!rtc.isrunning()) {
        Serial.println("RTC laeuft nicht - setze Zeit...");
        // Zeit auf Kompilierzeitpunkt setzen
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    Serial.println("DS1307/DS3231 Real-Time Clock");
    Serial.println("------------------------------");
}

void loop() {
    // Aktuelle Zeit lesen
    DateTime now = rtc.now();

    // Datum ausgeben
    Serial.print("Datum: ");
    if (now.day() < 10) Serial.print("0");
    Serial.print(now.day());
    Serial.print(".");
    if (now.month() < 10) Serial.print("0");
    Serial.print(now.month());
    Serial.print(".");
    Serial.print(now.year());
    Serial.print(" (");
    Serial.print(wochentage[now.dayOfTheWeek()]);
    Serial.println(")");

    // Zeit ausgeben
    Serial.print("Zeit:  ");
    if (now.hour() < 10) Serial.print("0");
    Serial.print(now.hour());
    Serial.print(":");
    if (now.minute() < 10) Serial.print("0");
    Serial.print(now.minute());
    Serial.print(":");
    if (now.second() < 10) Serial.print("0");
    Serial.println(now.second());

    // Unix-Timestamp
    Serial.print("Unix:  ");
    Serial.println(now.unixtime());

    Serial.println("--------------------");

    delay(1000);
}
