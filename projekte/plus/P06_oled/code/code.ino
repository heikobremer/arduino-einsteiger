// P06: OLED Dashboard mit analoger Uhr
// Multi-Sensor-Dashboard: SSD1306 (128x64), TM1637, DS1307, DHT22

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TM1637Display.h>
#include <RTClib.h>
#include <DHT.h>

// ===== DISPLAY =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== PIN-DEFINITIONEN =====
const int PIN_TM1637_CLK = 2;
const int PIN_TM1637_DIO = 3;
const int PIN_DHT = 4;

// ===== OBJEKTE =====
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);
RTC_DS1307 rtc;
DHT dht(PIN_DHT, DHT22);

// ===== STATE MACHINE =====
enum State {
    STATE_DEMO_TEXT,
    STATE_DEMO_SYMBOLS,
    STATE_DEMO_SHAPES,
    STATE_MAIN
};

State currentState = STATE_DEMO_TEXT;
unsigned long stateStart = 0;
const unsigned long DEMO_DURATION = 3000;

// ===== SENSOR-CACHE =====
float cachedTemp = 0.0;
float cachedHumid = 0.0;
bool sensorValid = false;
unsigned long lastSensorRead = 0;
const unsigned long SENSOR_INTERVAL = 2000;

// ===== TM1637 =====
bool colonOn = true;
unsigned long lastColonToggle = 0;

// ===== BITMAPS (PROGMEM) =====
// Thermometer 8x16
static const uint8_t PROGMEM bmpThermo[] = {
    0x1C, 0x22, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x2A, 0x2A, 0x2A, 0x49, 0x5D, 0x5D, 0x49, 0x3E
};

// Tropfen 8x16
static const uint8_t PROGMEM bmpDrop[] = {
    0x08, 0x08, 0x14, 0x14, 0x14, 0x22, 0x22, 0x41,
    0x41, 0x41, 0x41, 0x41, 0x22, 0x22, 0x14, 0x08
};

// Uhr 16x16
static const uint8_t PROGMEM bmpClock[] = {
    0x07, 0xE0, 0x18, 0x18, 0x20, 0x04, 0x40, 0x02,
    0x41, 0x02, 0x81, 0x01, 0x81, 0x01, 0x81, 0xF1,
    0x80, 0x01, 0x80, 0x01, 0x40, 0x02, 0x40, 0x02,
    0x20, 0x04, 0x18, 0x18, 0x07, 0xE0, 0x00, 0x00
};

// ===== DEMO-FUNKTIONEN =====

void demoText() {
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);

    oled.setTextSize(1);
    oled.setCursor(10, 2);
    oled.print(F("OLED Dashboard P06"));

    oled.setTextSize(2);
    oled.setCursor(10, 18);
    oled.print(F("128x64"));

    oled.setTextSize(3);
    oled.setCursor(10, 40);
    oled.print(F("OLED"));

    oled.display();
}

void demoSymbols() {
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);

    // Thermometer
    oled.drawBitmap(8, 24, bmpThermo, 8, 16, SSD1306_WHITE);
    oled.setTextSize(1);
    oled.setCursor(20, 28);
    oled.print(F("Temperatur"));

    // Tropfen
    oled.drawBitmap(48, 24, bmpDrop, 8, 16, SSD1306_WHITE);
    oled.setCursor(60, 28);
    oled.print(F("Feuchte"));

    // Uhr
    oled.drawBitmap(96, 24, bmpClock, 16, 16, SSD1306_WHITE);
    oled.setCursor(96, 44);
    oled.print(F("Uhr"));

    // Titel
    oled.setTextSize(1);
    oled.setCursor(20, 4);
    oled.print(F("Sensoren & Symbole"));

    oled.display();
}

void demoShapes() {
    oled.clearDisplay();

    // Rahmen
    oled.drawRect(0, 0, 128, 64, SSD1306_WHITE);

    // Gefuelltes Rechteck
    oled.fillRect(5, 5, 25, 15, SSD1306_WHITE);

    // Kreise
    oled.drawCircle(50, 32, 15, SSD1306_WHITE);
    oled.fillCircle(50, 32, 5, SSD1306_WHITE);

    // Dreieck
    oled.drawTriangle(80, 10, 70, 50, 90, 50, SSD1306_WHITE);

    // Linien
    oled.drawLine(100, 10, 120, 55, SSD1306_WHITE);
    oled.drawLine(105, 10, 125, 55, SSD1306_WHITE);

    oled.display();
}

// ===== ANALOGE UHR =====

void drawAnalogClock(int cx, int cy, int r, int h, int m) {
    // Ziffernblatt
    oled.drawCircle(cx, cy, r, SSD1306_WHITE);

    // 12 Stundenmarkierungen
    for (int i = 0; i < 12; i++) {
        float angle = i * 30.0 * DEG_TO_RAD;
        int x1 = cx + (int)((r - 1) * sin(angle));
        int y1 = cy - (int)((r - 1) * cos(angle));
        int x2 = cx + (int)((r - 4) * sin(angle));
        int y2 = cy - (int)((r - 4) * cos(angle));
        oled.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
    }

    // Mittelpunkt
    oled.fillCircle(cx, cy, 2, SSD1306_WHITE);

    // Stundenzeiger (55% Radius)
    float hourAngle = ((h % 12) * 30.0 + m * 0.5) * DEG_TO_RAD;
    int hx = cx + (int)(r * 0.55 * sin(hourAngle));
    int hy = cy - (int)(r * 0.55 * cos(hourAngle));
    oled.drawLine(cx, cy, hx, hy, SSD1306_WHITE);

    // Minutenzeiger (85% Radius)
    float minAngle = m * 6.0 * DEG_TO_RAD;
    int mx = cx + (int)(r * 0.85 * sin(minAngle));
    int my = cy - (int)(r * 0.85 * cos(minAngle));
    oled.drawLine(cx, cy, mx, my, SSD1306_WHITE);
}

// ===== HAUPTBILDSCHIRM 128x64 =====

void drawMainScreen64(DateTime now) {
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);

    // Analoge Uhr links (cx=24, cy=32, r=22)
    drawAnalogClock(24, 32, 22, now.hour(), now.minute());

    // Trennlinie
    oled.drawFastVLine(50, 0, 64, SSD1306_WHITE);

    // Rechte Seite: Digitale Uhrzeit (TextSize 2)
    oled.setTextSize(2);
    oled.setCursor(54, 2);
    char timeBuf[6];
    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d", now.hour(), now.minute());
    oled.print(timeBuf);

    // Datum (TextSize 1)
    oled.setTextSize(1);
    oled.setCursor(54, 22);
    char dateBuf[11];
    snprintf(dateBuf, sizeof(dateBuf), "%02d.%02d.%04d", now.day(), now.month(), now.year());
    oled.print(dateBuf);

    // Trennlinie horizontal
    oled.drawFastHLine(52, 34, 76, SSD1306_WHITE);

    // Temperatur
    oled.setCursor(54, 40);
    if (sensorValid) {
        oled.print(cachedTemp, 1);
        oled.drawCircle(96, 40, 1, SSD1306_WHITE);  // Grad-Symbol
        oled.setCursor(100, 40);
        oled.print(F("C"));
    } else {
        oled.print(F("-- "));
        oled.drawCircle(72, 40, 1, SSD1306_WHITE);
        oled.setCursor(76, 40);
        oled.print(F("C"));
    }

    // Luftfeuchtigkeit
    oled.setCursor(54, 52);
    if (sensorValid) {
        oled.print(cachedHumid, 1);
        oled.print(F(" %"));
    } else {
        oled.print(F("-- %"));
    }

    oled.display();
}

// ===== TM1637 =====

void updateTM1637(DateTime now) {
    // Doppelpunkt blinken
    if (millis() - lastColonToggle > 500) {
        colonOn = !colonOn;
        lastColonToggle = millis();
    }

    int displayValue = now.hour() * 100 + now.minute();
    uint8_t colonMask = colonOn ? 0b01000000 : 0b00000000;
    tm1637.showNumberDecEx(displayValue, colonMask, true);
}

// ===== SENSOREN =====

void readSensors() {
    if (millis() - lastSensorRead < SENSOR_INTERVAL) return;
    lastSensorRead = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
        cachedTemp = t;
        cachedHumid = h;
        sensorValid = true;
    }
}

// ===== SETUP =====

void setup() {
    Serial.begin(9600);
    Serial.println(F("P06 OLED Dashboard startet..."));

    // TM1637
    tm1637.setBrightness(0x0f);
    tm1637.clear();
    uint8_t loading[] = { SEG_D, SEG_D, SEG_D, SEG_D };
    tm1637.setSegments(loading);

    // DHT22
    dht.begin();

    // I2C + RTC
    Wire.begin();
    if (!rtc.begin()) {
        Serial.println(F("RTC nicht gefunden!"));
        uint8_t err[] = { SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, SEG_E | SEG_G, SEG_E | SEG_G, 0 };
        tm1637.setSegments(err);
        while (true);
    }

    if (!rtc.isrunning()) {
        Serial.println(F("RTC setze Zeit..."));
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // OLED
    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("OLED nicht gefunden!"));
        while (true);
    }

    oled.clearDisplay();
    oled.display();

    stateStart = millis();
    Serial.println(F("Initialisierung OK"));
}

// ===== HAUPTSCHLEIFE =====

void loop() {
    unsigned long elapsed = millis() - stateStart;

    switch (currentState) {
        case STATE_DEMO_TEXT:
            demoText();
            if (elapsed >= DEMO_DURATION) {
                currentState = STATE_DEMO_SYMBOLS;
                stateStart = millis();
            }
            break;

        case STATE_DEMO_SYMBOLS:
            demoSymbols();
            if (elapsed >= DEMO_DURATION) {
                currentState = STATE_DEMO_SHAPES;
                stateStart = millis();
            }
            break;

        case STATE_DEMO_SHAPES:
            demoShapes();
            if (elapsed >= DEMO_DURATION) {
                currentState = STATE_MAIN;
                Serial.println(F("Hauptbildschirm aktiv"));
            }
            break;

        case STATE_MAIN: {
            DateTime now = rtc.now();
            readSensors();
            drawMainScreen64(now);
            updateTM1637(now);
            break;
        }
    }

    delay(100);
}
