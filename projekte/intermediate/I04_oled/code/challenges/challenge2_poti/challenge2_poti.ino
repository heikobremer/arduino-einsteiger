// Challenge 2: Poti-Balkendiagramm auf OLED
// Zeigt den Potentiometer-Wert als Balken und Zahl an
// Benoetigt: OLED + Potentiometer an A0

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display-Konfiguration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

// Potentiometer an Analog-Pin
#define POTI_PIN A0

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Variablen fuer Glaettung
int letzterWert = 0;
const int GLAETTUNG = 5;  // Anzahl Samples fuer Mittelwert

void setup() {
    Serial.begin(9600);

    // Display initialisieren
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 nicht gefunden!");
        while (true);
    }

    Serial.println("Poti-Balkendiagramm");
    Serial.println("------------------------------");

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

int leseGeglaettet() {
    // Mehrere Messungen fuer stabilen Wert
    long summe = 0;
    for (int i = 0; i < GLAETTUNG; i++) {
        summe += analogRead(POTI_PIN);
        delay(2);
    }
    return summe / GLAETTUNG;
}

void zeigeBalken(int wert) {
    display.clearDisplay();

    // Rahmen
    display.drawRect(0, 0, 128, 64, SSD1306_WHITE);

    // Ueberschrift
    display.setTextSize(1);
    display.setCursor(25, 5);
    display.println("Potentiometer");

    // Balkenrahmen (120 Pixel breit fuer 0-1023)
    int balkenY = 25;
    int balkenHoehe = 20;
    display.drawRect(3, balkenY, 122, balkenHoehe, SSD1306_WHITE);

    // Gefuellter Balken (proportional zum Wert)
    int balkenBreite = map(wert, 0, 1023, 0, 118);
    if (balkenBreite > 0) {
        display.fillRect(5, balkenY + 2, balkenBreite, balkenHoehe - 4, SSD1306_WHITE);
    }

    // Prozent-Anzeige
    int prozent = map(wert, 0, 1023, 0, 100);
    display.setTextSize(2);
    display.setCursor(45, 48);
    if (prozent < 10) display.print(" ");
    if (prozent < 100) display.print(" ");
    display.print(prozent);
    display.setTextSize(1);
    display.print("%");

    // Rohwert klein anzeigen
    display.setTextSize(1);
    display.setCursor(3, 48);
    display.print(wert);

    // Skala unter Balken
    display.setCursor(3, balkenY + balkenHoehe + 2);
    display.print("0");
    display.setCursor(55, balkenY + balkenHoehe + 2);
    display.print("512");
    display.setCursor(105, balkenY + balkenHoehe + 2);
    display.print("1023");

    display.display();
}

void loop() {
    // Poti-Wert lesen (geglaettet)
    int potiWert = leseGeglaettet();

    // Nur aktualisieren bei Aenderung (vermeidet Flackern)
    if (abs(potiWert - letzterWert) > 5) {
        letzterWert = potiWert;

        Serial.print("Poti: ");
        Serial.print(potiWert);
        Serial.print(" (");
        Serial.print(map(potiWert, 0, 1023, 0, 100));
        Serial.println("%)");
    }

    zeigeBalken(potiWert);

    delay(50);
}
