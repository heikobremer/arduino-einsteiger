// P06: OLED Dashboard (Hardware-Version)
// Waveshare 0.91" SSD1306 128x32 (Chip im 128x64-Modus, nur obere 32px sichtbar)
// TM1637, DHT22, Software-Uhr (ohne RTC)
// U8g2 Library

#include <Wire.h>
#include <U8g2lib.h>
#include <TM1637Display.h>
#include <DHT.h>

// ===== DISPLAY =====
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C oled(U8G2_R0);

// ===== PIN-DEFINITIONEN =====
const int PIN_TM1637_CLK = 2;
const int PIN_TM1637_DIO = 3;
const int PIN_DHT = 4;

// ===== OBJEKTE =====
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);
DHT dht(PIN_DHT, DHT22);

// ===== STATE MACHINE =====
enum State {
    STATE_DEMO_TEXT,
    STATE_DEMO_SHAPES,
    STATE_MAIN
};

State currentState = STATE_DEMO_TEXT;
unsigned long stateStart = 0;
const unsigned long DEMO_DURATION = 3000;

// ===== SOFTWARE-UHR =====
uint8_t swHour, swMinute, swSecond;
uint8_t swDay, swMonth;
uint16_t swYear;
unsigned long lastClockTick = 0;

void initSoftwareClock() {
    swHour   = (__TIME__[0] - '0') * 10 + (__TIME__[1] - '0');
    swMinute = (__TIME__[3] - '0') * 10 + (__TIME__[4] - '0');
    swSecond = (__TIME__[6] - '0') * 10 + (__TIME__[7] - '0');
    swDay = (__DATE__[4] == ' ' ? 0 : (__DATE__[4] - '0') * 10) + (__DATE__[5] - '0');
    swYear = (__DATE__[7] - '0') * 1000 + (__DATE__[8] - '0') * 100
           + (__DATE__[9] - '0') * 10   + (__DATE__[10] - '0');
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char m0 = __DATE__[0], m1 = __DATE__[1], m2 = __DATE__[2];
    swMonth = 1;
    for (uint8_t i = 0; i < 12; i++) {
        if (m0 == months[i*3] && m1 == months[i*3+1] && m2 == months[i*3+2]) {
            swMonth = i + 1;
            break;
        }
    }
}

uint8_t daysInMonth(uint8_t month) {
    const uint8_t dim[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (month < 1 || month > 12) return 30;
    return dim[month - 1];
}

void tickSoftwareClock() {
    if (millis() - lastClockTick < 1000) return;
    lastClockTick += 1000;
    swSecond++;
    if (swSecond >= 60) { swSecond = 0; swMinute++;
        if (swMinute >= 60) { swMinute = 0; swHour++;
            if (swHour >= 24) { swHour = 0; swDay++;
                if (swDay > daysInMonth(swMonth)) { swDay = 1; swMonth++;
                    if (swMonth > 12) { swMonth = 1; swYear++; }
                }
            }
        }
    }
}

// ===== SENSOR-CACHE =====
float cachedTemp = 0.0;
float cachedHumid = 0.0;
bool sensorValid = false;
unsigned long lastSensorRead = 0;
const unsigned long SENSOR_INTERVAL = 2000;

// ===== TM1637 =====
bool colonOn = true;
unsigned long lastColonToggle = 0;

// ===== DEMO =====

void demoText() {
    oled.clearBuffer();
    oled.setFont(u8g2_font_6x10_tr);
    oled.drawStr(4, 10, "OLED Dashboard P06");
    oled.setFont(u8g2_font_ncenB14_tr);
    oled.drawStr(4, 30, "Waveshare");
    oled.sendBuffer();
}

void demoShapes() {
    oled.clearBuffer();
    oled.drawFrame(0, 0, 128, 32);
    oled.drawBox(5, 5, 18, 10);
    oled.drawCircle(40, 16, 12);
    oled.drawDisc(40, 16, 4);
    oled.drawTriangle(65, 4, 55, 27, 75, 27);
    oled.drawLine(85, 4, 105, 27);
    oled.drawLine(90, 4, 110, 27);
    oled.drawLine(95, 4, 115, 27);
    oled.sendBuffer();
}

// ===== HAUPTBILDSCHIRM =====
// Layout 128x32:
//
//  2px Rand oben
// +-------------------------------+
// |  14:30         04.02.2026     |  Baseline y=12
// |───────────────────────────────|  Trennlinie y=16
// |  23.5°C           45.2 %     |  Baseline y=28
// +-------------------------------+
//  2px Rand unten

void drawMainScreen() {
    oled.clearBuffer();

    // --- Zeile 1: Uhrzeit (gross) + Datum (klein) ---
    char timeBuf[6];
    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d", swHour, swMinute);
    oled.setFont(u8g2_font_ncenB10_tr);
    oled.drawStr(2, 12, timeBuf);

    char dateBuf[11];
    snprintf(dateBuf, sizeof(dateBuf), "%02d.%02d.%04d", swDay, swMonth, swYear);
    oled.setFont(u8g2_font_6x10_tr);
    oled.drawStr(68, 11, dateBuf);

    // Trennlinie
    oled.drawHLine(0, 16, 128);

    // --- Zeile 2: Temperatur + Luftfeuchtigkeit ---
    oled.setFont(u8g2_font_6x10_tr);

    if (sensorValid) {
        char tempBuf[8];
        dtostrf(cachedTemp, 4, 1, tempBuf);
        oled.drawStr(2, 28, tempBuf);
        int tx = 2 + strlen(tempBuf) * 6;
        oled.drawCircle(tx + 1, 19, 1);
        oled.drawStr(tx + 4, 28, "C");
    } else {
        oled.drawStr(2, 28, "--.-");
        oled.drawCircle(28, 19, 1);
        oled.drawStr(31, 28, "C");
    }

    if (sensorValid) {
        char humBuf[10];
        dtostrf(cachedHumid, 4, 1, humBuf);
        strcat(humBuf, " %");
        oled.drawStr(80, 28, humBuf);
    } else {
        oled.drawStr(80, 28, "--.- %");
    }

    oled.sendBuffer();
}

// ===== TM1637 =====

void updateTM1637() {
    if (millis() - lastColonToggle > 500) {
        colonOn = !colonOn;
        lastColonToggle = millis();
    }
    int displayValue = swHour * 100 + swMinute;
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
    delay(500);
    Serial.println(F("P06 OLED Dashboard startet..."));

    // TM1637
    tm1637.setBrightness(0x0f);
    tm1637.clear();
    uint8_t loading[] = { SEG_D, SEG_D, SEG_D, SEG_D };
    tm1637.setSegments(loading);

    // DHT22
    dht.begin();

    // Software-Uhr
    initSoftwareClock();
    lastClockTick = millis();

    // OLED
    oled.begin();

    stateStart = millis();
    Serial.println(F("OK"));
}

// ===== HAUPTSCHLEIFE =====

void loop() {
    tickSoftwareClock();
    unsigned long elapsed = millis() - stateStart;

    switch (currentState) {
        case STATE_DEMO_TEXT:
            demoText();
            if (elapsed >= DEMO_DURATION) {
                currentState = STATE_DEMO_SHAPES;
                stateStart = millis();
            }
            break;
        case STATE_DEMO_SHAPES:
            demoShapes();
            if (elapsed >= DEMO_DURATION) {
                currentState = STATE_MAIN;
            }
            break;
        case STATE_MAIN:
            readSensors();
            drawMainScreen();
            updateTM1637();
            break;
    }
    delay(100);
}
