// Hardware-Projekt: Klimastation mit TM1637 und DHT22
// Arduino Uno R4 - Uhrzeit, Temperatur und Humidity im Wechsel
// Anzeige rotiert alle 2 Sekunden: HH:MM -> t:XX -> h:XX

#include <TM1637Display.h>
#include <DHT.h>
#include "RTC.h"

// ===== PIN-DEFINITIONEN (identisch zu P03) =====
const int PIN_TM1637_CLK = 2;
const int PIN_TM1637_DIO = 3;
const int PIN_DHT = 4;

// ===== OBJEKTE =====
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);
DHT dht(PIN_DHT, DHT22);

// ===== ANZEIGE-MODI =====
enum DisplayMode { SHOW_TIME, SHOW_TEMP, SHOW_HUMID };
DisplayMode currentDisplay = SHOW_TIME;

// ===== TIMING =====
unsigned long lastDisplaySwitch = 0;
const unsigned long DISPLAY_INTERVAL = 2000;

// Sensor wird einmal pro Turnus gelesen (beim Wechsel auf SHOW_TEMP)

// ===== MESSWERTE =====
float temperature = 0;
float humidity = 0;
bool sensorValid = false;

// ===== SEGMENT-DEFINITIONEN =====
// Kleinbuchstabe 't': Segmente d, e, f, g
const uint8_t SEG_t = SEG_D | SEG_E | SEG_F | SEG_G;
// Kleinbuchstabe 'h': Segmente c, e, f, g
const uint8_t SEG_h = SEG_C | SEG_E | SEG_F | SEG_G;

// ===== RTC ZEIT SETZEN =====
// Parst __DATE__ und __TIME__ fuer den eingebauten RTC
void setRTCFromCompileTime() {
    // __DATE__ Format: "Feb  3 2026"
    // __TIME__ Format: "14:30:00"
    const char* date = __DATE__;
    const char* time = __TIME__;

    // Monat parsen
    const char* months[] = { "Jan","Feb","Mar","Apr","May","Jun",
                             "Jul","Aug","Sep","Oct","Nov","Dec" };
    int month = 1;
    for (int i = 0; i < 12; i++) {
        if (strncmp(date, months[i], 3) == 0) {
            month = i + 1;
            break;
        }
    }

    int day = atoi(date + 4);
    int year = atoi(date + 7);
    int hour = atoi(time);
    int minute = atoi(time + 3);
    int second = atoi(time + 6);

    RTCTime startTime(day, (Month)month, year,
                      hour, minute, second,
                      DayOfWeek::MONDAY,
                      SaveLight::SAVING_TIME_INACTIVE);
    RTC.setTime(startTime);
}

// ===== ANZEIGE-FUNKTIONEN =====

void showTime() {
    RTCTime currentTime;
    RTC.getTime(currentTime);

    int h = currentTime.getHour();
    int m = currentTime.getMinutes();
    int displayValue = h * 100 + m;

    // Doppelpunkt an bei Zeitanzeige
    display.showNumberDecEx(displayValue, 0b01000000, true);
}

void showTemperature() {
    int tempInt = (int)(temperature + 0.5);

    uint8_t segments[4];
    segments[0] = SEG_t;                                   // 't'
    segments[1] = 0;                                       // leer
    segments[2] = display.encodeDigit(tempInt / 10);       // Zehner
    segments[3] = display.encodeDigit(tempInt % 10);       // Einer

    display.setSegments(segments);
}

void showHumidity() {
    int humidInt = (int)(humidity + 0.5);

    uint8_t segments[4];
    segments[0] = SEG_h;                                   // 'h'
    segments[1] = 0;                                       // leer
    segments[2] = display.encodeDigit(humidInt / 10);      // Zehner
    segments[3] = display.encodeDigit(humidInt % 10);      // Einer

    display.setSegments(segments);
}

// ===== SETUP =====

void setup() {
    Serial.begin(9600);
    Serial.println("Klimastation Hardware startet...");

    // TM1637 initialisieren
    display.setBrightness(0x0f);
    display.clear();

    // Ladebalken anzeigen
    uint8_t loading[] = { SEG_D, SEG_D, SEG_D, SEG_D };
    display.setSegments(loading);

    // DHT22 initialisieren
    dht.begin();

    // Eingebauten RTC initialisieren
    RTC.begin();
    setRTCFromCompileTime();

    Serial.println("RTC gesetzt auf Kompilierzeit");

    delay(1000);
    display.clear();
}

// ===== HAUPTSCHLEIFE =====

void loop() {
    // Anzeige umschalten
    if (millis() - lastDisplaySwitch >= DISPLAY_INTERVAL) {
        lastDisplaySwitch = millis();

        switch (currentDisplay) {
            case SHOW_TIME:
                // Sensor einmal pro Turnus lesen (vor Temperaturanzeige)
                {
                    float t = dht.readTemperature();
                    float h = dht.readHumidity();
                    if (!isnan(t) && !isnan(h)) {
                        temperature = t;
                        humidity = h;
                        sensorValid = true;
                        Serial.print("Temp: ");
                        Serial.print(temperature);
                        Serial.print(" C, Humid: ");
                        Serial.print(humidity);
                        Serial.println(" %");
                    }
                }
                currentDisplay = SHOW_TEMP;
                break;
            case SHOW_TEMP:
                currentDisplay = SHOW_HUMID;
                break;
            case SHOW_HUMID:
                currentDisplay = SHOW_TIME;
                break;
        }
    }

    // Aktuelle Anzeige aktualisieren
    switch (currentDisplay) {
        case SHOW_TIME:
            showTime();
            break;
        case SHOW_TEMP:
            if (sensorValid) showTemperature();
            break;
        case SHOW_HUMID:
            if (sensorValid) showHumidity();
            break;
    }

    delay(100);
}
