// I04: SSD1306 OLED Display mit I2C
// 128x64 Pixel Display fuer Text und Grafik

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display-Groesse
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(9600);

    // I2C auf A4 (SDA) und A5 (SCL) - Standard Arduino Uno
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 nicht gefunden!");
        while (true);
    }

    Serial.println("SSD1306 OLED Display");
    Serial.println("------------------------------");

    display.clearDisplay();
    display.display();
}

void demoText() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(20, 5);
    display.println("Hallo Welt!");

    display.setCursor(20, 20);
    display.println("Arduino Uno");

    display.setCursor(15, 35);
    display.println("C++ auf dem Uno");

    display.setCursor(20, 50);
    display.println("128x64 OLED");

    display.display();
}

void demoShapes() {
    display.clearDisplay();

    // Rahmen
    display.drawRect(0, 0, 128, 64, SSD1306_WHITE);

    // Gefuelltes Rechteck
    display.fillRect(10, 10, 30, 20, SSD1306_WHITE);

    // Linie
    display.drawLine(50, 10, 120, 50, SSD1306_WHITE);

    // Horizontale Linie
    display.drawFastHLine(50, 30, 70, SSD1306_WHITE);

    // Vertikale Linie
    display.drawFastVLine(85, 10, 40, SSD1306_WHITE);

    display.display();
}

void demoAnimation() {
    for (int x = 0; x < 100; x += 5) {
        display.clearDisplay();

        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(25, 25);
        display.println("Animation!");

        display.fillRect(x, 45, 20, 10, SSD1306_WHITE);

        display.display();
        delay(100);
    }
}

void demoCounter() {
    for (int i = 0; i < 10; i++) {
        display.clearDisplay();

        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(35, 15);
        display.println("Zaehler:");

        display.setTextSize(2);
        display.setCursor(55, 35);
        display.println(i);

        display.drawRect(0, 0, 128, 64, SSD1306_WHITE);

        display.display();
        delay(500);
    }
}

void loop() {
    Serial.println("  Text-Demo");
    demoText();
    delay(3000);

    Serial.println("  Formen-Demo");
    demoShapes();
    delay(3000);

    Serial.println("  Animation");
    demoAnimation();
    delay(1000);

    Serial.println("  Zaehler");
    demoCounter();
    delay(1000);
}
