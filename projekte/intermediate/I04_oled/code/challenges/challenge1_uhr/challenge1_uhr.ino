// Challenge 1: Digitale Uhr auf OLED
// Zeigt Stunden:Minuten:Sekunden mit millis()
// Verwendet das gleiche OLED wie main.ino

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display-Konfiguration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Zeit-Variablen
unsigned long previousMillis = 0;
int sekunden = 0;
int minuten = 0;
int stunden = 0;

void setup() {
    Serial.begin(9600);

    // Display initialisieren
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 nicht gefunden!");
        while (true);
    }

    Serial.println("Digitale OLED Uhr");
    Serial.println("------------------------------");

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void zeigeUhr() {
    display.clearDisplay();

    // Rahmen zeichnen
    display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
    display.drawRect(2, 2, 124, 60, SSD1306_WHITE);

    // Ueberschrift
    display.setTextSize(1);
    display.setCursor(40, 8);
    display.println("Uhrzeit");

    // Zeit formatieren (mit fuehrenden Nullen)
    char zeitBuffer[9];
    sprintf(zeitBuffer, "%02d:%02d:%02d", stunden, minuten, sekunden);

    // Grosse Zeitanzeige
    display.setTextSize(2);
    display.setCursor(16, 30);
    display.println(zeitBuffer);

    // Laufzeit in Sekunden unten anzeigen
    display.setTextSize(1);
    display.setCursor(30, 52);
    unsigned long gesamt = (unsigned long)stunden * 3600 + minuten * 60 + sekunden;
    display.print("Gesamt: ");
    display.print(gesamt);
    display.println("s");

    display.display();
}

void aktualisiereZeit() {
    unsigned long currentMillis = millis();

    // Jede Sekunde aktualisieren
    if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;

        sekunden++;

        // Sekunden -> Minuten
        if (sekunden >= 60) {
            sekunden = 0;
            minuten++;

            // Minuten -> Stunden
            if (minuten >= 60) {
                minuten = 0;
                stunden++;

                // 24-Stunden-Format
                if (stunden >= 24) {
                    stunden = 0;
                }
            }
        }

        // Serielle Ausgabe
        Serial.print("Zeit: ");
        if (stunden < 10) Serial.print("0");
        Serial.print(stunden);
        Serial.print(":");
        if (minuten < 10) Serial.print("0");
        Serial.print(minuten);
        Serial.print(":");
        if (sekunden < 10) Serial.print("0");
        Serial.println(sekunden);
    }
}

void loop() {
    aktualisiereZeit();
    zeigeUhr();

    // Kleine Pause um CPU zu schonen
    delay(100);
}
