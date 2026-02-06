// Challenge 3: Einfaches Pong-Spiel auf OLED
// Steuere das Paddle mit 2 Tastern (Auf/Ab)
// Benoetigt: OLED + 2 Taster an Pin 2 und 3

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display-Konfiguration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

// Taster-Pins
#define BTN_AUF 2    // Paddle nach oben
#define BTN_AB 3     // Paddle nach unten

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Ball-Eigenschaften
int ballX = 64;
int ballY = 32;
int ballSpeedX = 2;
int ballSpeedY = 1;
const int BALL_SIZE = 4;

// Paddle-Eigenschaften (Spieler links)
int paddleY = 25;
const int PADDLE_WIDTH = 4;
const int PADDLE_HEIGHT = 16;
const int PADDLE_X = 5;
const int PADDLE_SPEED = 3;

// Computer-Paddle (rechts)
int cpuPaddleY = 25;
const int CPU_PADDLE_X = 119;

// Spielstand
int spielerPunkte = 0;
int cpuPunkte = 0;

// Timing
unsigned long letzteAktualisierung = 0;
const int FRAME_DELAY = 30;  // ~33 FPS

void setup() {
    Serial.begin(9600);

    // Taster als Eingaenge mit Pull-Up
    pinMode(BTN_AUF, INPUT_PULLUP);
    pinMode(BTN_AB, INPUT_PULLUP);

    // Display initialisieren
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 nicht gefunden!");
        while (true);
    }

    Serial.println("OLED Pong-Spiel");
    Serial.println("Taster: Pin 2 = Auf, Pin 3 = Ab");
    Serial.println("------------------------------");

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    // Startbildschirm
    display.setTextSize(2);
    display.setCursor(35, 20);
    display.println("PONG");
    display.setTextSize(1);
    display.setCursor(20, 45);
    display.println("Druecke einen Taster");
    display.display();

    // Warten auf Tastendruck
    while (digitalRead(BTN_AUF) && digitalRead(BTN_AB)) {
        delay(10);
    }
    delay(500);
}

void bewegeSpielerPaddle() {
    // Taster lesen (LOW = gedrueckt wegen Pull-Up)
    if (digitalRead(BTN_AUF) == LOW) {
        paddleY -= PADDLE_SPEED;
    }
    if (digitalRead(BTN_AB) == LOW) {
        paddleY += PADDLE_SPEED;
    }

    // Grenzen pruefen
    if (paddleY < 0) paddleY = 0;
    if (paddleY > SCREEN_HEIGHT - PADDLE_HEIGHT) {
        paddleY = SCREEN_HEIGHT - PADDLE_HEIGHT;
    }
}

void bewegeCpuPaddle() {
    // Einfache KI: Folgt dem Ball mit Verzoegerung
    int paddleMitte = cpuPaddleY + PADDLE_HEIGHT / 2;

    if (paddleMitte < ballY - 2) {
        cpuPaddleY += 2;  // Langsamer als Spieler
    } else if (paddleMitte > ballY + 2) {
        cpuPaddleY -= 2;
    }

    // Grenzen pruefen
    if (cpuPaddleY < 0) cpuPaddleY = 0;
    if (cpuPaddleY > SCREEN_HEIGHT - PADDLE_HEIGHT) {
        cpuPaddleY = SCREEN_HEIGHT - PADDLE_HEIGHT;
    }
}

void bewegeBall() {
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Oben/Unten abprallen
    if (ballY <= 0 || ballY >= SCREEN_HEIGHT - BALL_SIZE) {
        ballSpeedY = -ballSpeedY;
        ballY = constrain(ballY, 0, SCREEN_HEIGHT - BALL_SIZE);
    }

    // Spieler-Paddle Kollision (links)
    if (ballX <= PADDLE_X + PADDLE_WIDTH &&
        ballX >= PADDLE_X &&
        ballY + BALL_SIZE >= paddleY &&
        ballY <= paddleY + PADDLE_HEIGHT) {
        ballSpeedX = abs(ballSpeedX);  // Nach rechts
        ballX = PADDLE_X + PADDLE_WIDTH + 1;

        // Winkel aendern je nach Treffpunkt
        int trefferPunkt = ballY - paddleY;
        ballSpeedY = map(trefferPunkt, 0, PADDLE_HEIGHT, -3, 3);
    }

    // CPU-Paddle Kollision (rechts)
    if (ballX + BALL_SIZE >= CPU_PADDLE_X &&
        ballX <= CPU_PADDLE_X + PADDLE_WIDTH &&
        ballY + BALL_SIZE >= cpuPaddleY &&
        ballY <= cpuPaddleY + PADDLE_HEIGHT) {
        ballSpeedX = -abs(ballSpeedX);  // Nach links
        ballX = CPU_PADDLE_X - BALL_SIZE - 1;
    }

    // Punkt fuer Spieler (Ball rechts raus)
    if (ballX > SCREEN_WIDTH) {
        spielerPunkte++;
        Serial.print("Spieler Punkt! Stand: ");
        Serial.print(spielerPunkte);
        Serial.print(" - ");
        Serial.println(cpuPunkte);
        resetBall();
    }

    // Punkt fuer CPU (Ball links raus)
    if (ballX < 0) {
        cpuPunkte++;
        Serial.print("CPU Punkt! Stand: ");
        Serial.print(spielerPunkte);
        Serial.print(" - ");
        Serial.println(cpuPunkte);
        resetBall();
    }
}

void resetBall() {
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballSpeedX = (ballSpeedX > 0) ? -2 : 2;  // Richtung wechseln
    ballSpeedY = random(-2, 3);
    delay(500);
}

void zeichneSpielfeld() {
    display.clearDisplay();

    // Mittellinie (gestrichelt)
    for (int y = 0; y < SCREEN_HEIGHT; y += 8) {
        display.drawFastVLine(SCREEN_WIDTH / 2, y, 4, SSD1306_WHITE);
    }

    // Spielstand oben
    display.setTextSize(1);
    display.setCursor(50, 2);
    display.print(spielerPunkte);
    display.print("-");
    display.print(cpuPunkte);

    // Spieler-Paddle (links)
    display.fillRect(PADDLE_X, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);

    // CPU-Paddle (rechts)
    display.fillRect(CPU_PADDLE_X, cpuPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);

    // Ball
    display.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);

    display.display();
}

void loop() {
    unsigned long jetzt = millis();

    if (jetzt - letzteAktualisierung >= FRAME_DELAY) {
        letzteAktualisierung = jetzt;

        bewegeSpielerPaddle();
        bewegeCpuPaddle();
        bewegeBall();
        zeichneSpielfeld();
    }
}
