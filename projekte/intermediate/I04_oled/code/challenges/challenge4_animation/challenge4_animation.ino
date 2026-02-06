// Challenge 4: Bouncing Ball Animation auf OLED
// Ein Ball prallt an allen Raendern ab
// Nur OLED erforderlich (gleiche Verkabelung wie main.ino)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display-Konfiguration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Ball-Eigenschaften
float ballX = 64.0;
float ballY = 32.0;
float speedX = 2.5;
float speedY = 1.8;
const int BALL_RADIUS = 5;

// Spur des Balls (fuer Effekt)
const int SPUR_LAENGE = 5;
int spurX[SPUR_LAENGE];
int spurY[SPUR_LAENGE];
int spurIndex = 0;

// Timing
unsigned long letzteAktualisierung = 0;
const int FRAME_DELAY = 25;  // 40 FPS

// Anzahl Kollisionen
int kollisionen = 0;

void setup() {
    Serial.begin(9600);

    // Display initialisieren
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 nicht gefunden!");
        while (true);
    }

    Serial.println("Bouncing Ball Animation");
    Serial.println("------------------------------");

    // Spur initialisieren
    for (int i = 0; i < SPUR_LAENGE; i++) {
        spurX[i] = (int)ballX;
        spurY[i] = (int)ballY;
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void aktualisiereSpur() {
    // Aktuelle Position in Spur speichern
    spurX[spurIndex] = (int)ballX;
    spurY[spurIndex] = (int)ballY;
    spurIndex = (spurIndex + 1) % SPUR_LAENGE;
}

void bewegeBall() {
    // Position aktualisieren
    ballX += speedX;
    ballY += speedY;

    bool kollision = false;

    // Linke/Rechte Wand
    if (ballX <= BALL_RADIUS) {
        ballX = BALL_RADIUS;
        speedX = -speedX;
        kollision = true;
    }
    if (ballX >= SCREEN_WIDTH - BALL_RADIUS) {
        ballX = SCREEN_WIDTH - BALL_RADIUS;
        speedX = -speedX;
        kollision = true;
    }

    // Oben/Unten
    if (ballY <= BALL_RADIUS) {
        ballY = BALL_RADIUS;
        speedY = -speedY;
        kollision = true;
    }
    if (ballY >= SCREEN_HEIGHT - BALL_RADIUS) {
        ballY = SCREEN_HEIGHT - BALL_RADIUS;
        speedY = -speedY;
        kollision = true;
    }

    if (kollision) {
        kollisionen++;
        Serial.print("Kollision #");
        Serial.print(kollisionen);
        Serial.print(" bei (");
        Serial.print((int)ballX);
        Serial.print(", ");
        Serial.print((int)ballY);
        Serial.println(")");

        // Leichte Zufallsvariation bei Kollision
        speedX += (random(-10, 11) / 100.0);
        speedY += (random(-10, 11) / 100.0);

        // Geschwindigkeit begrenzen
        speedX = constrain(speedX, -4.0, 4.0);
        speedY = constrain(speedY, -4.0, 4.0);

        // Mindestgeschwindigkeit
        if (abs(speedX) < 1.0) speedX = (speedX > 0) ? 1.0 : -1.0;
        if (abs(speedY) < 1.0) speedY = (speedY > 0) ? 1.0 : -1.0;
    }

    aktualisiereSpur();
}

void zeichneBall() {
    display.clearDisplay();

    // Rahmen
    display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

    // Spur zeichnen (immer kleiner werdende Kreise)
    for (int i = 0; i < SPUR_LAENGE; i++) {
        int idx = (spurIndex + i) % SPUR_LAENGE;
        int radius = map(i, 0, SPUR_LAENGE - 1, 1, BALL_RADIUS - 2);
        if (radius > 0) {
            display.drawCircle(spurX[idx], spurY[idx], radius, SSD1306_WHITE);
        }
    }

    // Ball (gefuellt)
    display.fillCircle((int)ballX, (int)ballY, BALL_RADIUS, SSD1306_WHITE);

    // Info oben links
    display.setTextSize(1);
    display.setCursor(3, 3);
    display.print("Bounces: ");
    display.print(kollisionen);

    // Geschwindigkeit unten
    display.setCursor(3, 55);
    display.print("vX:");
    display.print(speedX, 1);
    display.print(" vY:");
    display.print(speedY, 1);

    display.display();
}

void loop() {
    unsigned long jetzt = millis();

    if (jetzt - letzteAktualisierung >= FRAME_DELAY) {
        letzteAktualisierung = jetzt;

        bewegeBall();
        zeichneBall();
    }
}
