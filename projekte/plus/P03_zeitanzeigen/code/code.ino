// P03: Klimastation mit TM1637, DHT22 und NeoPixels
// Visuelle Anzeige von Uhrzeit, Temperatur und Luftfeuchtigkeit

#include <TM1637Display.h>
#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

// ===== PIN-DEFINITIONEN =====
const int PIN_TM1637_CLK = 2;   // TM1637 Clock
const int PIN_TM1637_DIO = 3;   // TM1637 Data
const int PIN_DHT = 4;          // DHT22 Datenpin
const int PIN_NEOPIXEL_TEMP = 5;   // NeoPixel-Streifen für Temperatur
const int PIN_NEOPIXEL_HUMID = 6;  // NeoPixel-Streifen für Humidity

// ===== KONFIGURATION =====
const int NUM_LEDS = 10;         // Anzahl LEDs pro Streifen

// Temperatur-Bereich für Skalierung (Celsius)
const float TEMP_MIN = 15.0;     // Minimum (ganz links, rot)
const float TEMP_MAX = 30.0;     // Maximum (ganz rechts, rot)
const float TEMP_OPTIMAL_LOW = 20.0;   // Optimaler Bereich Start
const float TEMP_OPTIMAL_HIGH = 24.0;  // Optimaler Bereich Ende

// Humidity-Bereich für Skalierung (Prozent)
const float HUMID_MIN = 30.0;    // Minimum (ganz links, rot)
const float HUMID_MAX = 70.0;    // Maximum (ganz rechts, rot)
const float HUMID_OPTIMAL_LOW = 40.0;  // Optimaler Bereich Start
const float HUMID_OPTIMAL_HIGH = 60.0; // Optimaler Bereich Ende

// ===== OBJEKTE =====
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);
RTC_DS1307 rtc;
DHT dht(PIN_DHT, DHT22);

Adafruit_NeoPixel stripTemp(NUM_LEDS, PIN_NEOPIXEL_TEMP, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripHumid(NUM_LEDS, PIN_NEOPIXEL_HUMID, NEO_GRB + NEO_KHZ800);

// ===== TIMING =====
unsigned long lastSensorRead = 0;
const unsigned long SENSOR_INTERVAL = 2000;  // 2 Sekunden

bool colonOn = true;  // Für blinkenden Doppelpunkt

// ===== HILFSFUNKTIONEN =====

// Verschiebt logische Position auf dem Ring,
// damit optimale Werte (Mitte der Skala) oben liegen (12-Uhr)
const int RING_OFFSET = 5;  // Halber Ring = 180 Grad Drehung

int logicalToPhysical(int logicalPos) {
    return (logicalPos + RING_OFFSET) % NUM_LEDS;
}

// Mappt einen Wert auf eine Position (0 bis NUM_LEDS-1)
int mapToPosition(float value, float minVal, float maxVal) {
    int pos = (int)((value - minVal) / (maxVal - minVal) * (NUM_LEDS - 1));
    return constrain(pos, 0, NUM_LEDS - 1);
}

// Berechnet die Farbe basierend auf Position und Optimal-Bereich
// Farbverlauf: Rot -> Orange -> Gelb -> Grün -> Gelb -> Orange -> Rot
uint32_t getScaleColor(Adafruit_NeoPixel &strip, int position, float value,
                        float minVal, float maxVal,
                        float optimalLow, float optimalHigh) {

    // Normalisierte Position (0.0 - 1.0)
    float normalized = (float)position / (NUM_LEDS - 1);
    float valueNormalized = (value - minVal) / (maxVal - minVal);

    // Berechne Distanz zum optimalen Bereich
    float optLowNorm = (optimalLow - minVal) / (maxVal - minVal);
    float optHighNorm = (optimalHigh - minVal) / (maxVal - minVal);

    float distFromOptimal;
    if (normalized < optLowNorm) {
        distFromOptimal = optLowNorm - normalized;
    } else if (normalized > optHighNorm) {
        distFromOptimal = normalized - optHighNorm;
    } else {
        distFromOptimal = 0;  // Im optimalen Bereich
    }

    // Farbe basierend auf Distanz
    // 0 = Grün, 0.1-0.2 = Gelb, 0.3+ = Orange/Rot
    if (distFromOptimal < 0.05) {
        return strip.Color(0, 255, 0);      // Grün - optimal
    } else if (distFromOptimal < 0.15) {
        return strip.Color(128, 255, 0);    // Gelbgrün
    } else if (distFromOptimal < 0.25) {
        return strip.Color(255, 255, 0);    // Gelb
    } else if (distFromOptimal < 0.35) {
        return strip.Color(255, 128, 0);    // Orange
    } else {
        return strip.Color(255, 0, 0);      // Rot - kritisch
    }
}

// Einfachere Variante: Feste Farbskala Rot-Gelb-Grün-Gelb-Rot
uint32_t getFixedScaleColor(Adafruit_NeoPixel &strip, int position) {
    // Position 0-1: Rot (zu niedrig)
    // Position 2-3: Orange/Gelb
    // Position 4-5: Grün (optimal)
    // Position 6-7: Orange/Gelb
    // Position 8-9: Rot (zu hoch)

    switch (position) {
        case 0:
        case 9:
            return strip.Color(255, 0, 0);      // Rot
        case 1:
        case 8:
            return strip.Color(255, 80, 0);     // Orange-Rot
        case 2:
        case 7:
            return strip.Color(255, 165, 0);    // Orange
        case 3:
        case 6:
            return strip.Color(255, 255, 0);    // Gelb
        case 4:
        case 5:
            return strip.Color(0, 255, 0);      // Grün
        default:
            return strip.Color(0, 0, 0);
    }
}

// Aktualisiert den Temperatur-NeoPixel-Streifen
void updateTemperatureLeds(float temp) {
    int activePosition = mapToPosition(temp, TEMP_MIN, TEMP_MAX);

    for (int i = 0; i < NUM_LEDS; i++) {
        int pixel = logicalToPhysical(i);
        if (i <= activePosition) {
            stripTemp.setPixelColor(pixel, getFixedScaleColor(stripTemp, i));
        } else {
            stripTemp.setPixelColor(pixel, 0);
        }
    }
    stripTemp.show();
}

// Aktualisiert den Humidity-NeoPixel-Streifen
void updateHumidityLeds(float humid) {
    int activePosition = mapToPosition(humid, HUMID_MIN, HUMID_MAX);

    for (int i = 0; i < NUM_LEDS; i++) {
        int pixel = logicalToPhysical(i);
        if (i <= activePosition) {
            stripHumid.setPixelColor(pixel, getFixedScaleColor(stripHumid, i));
        } else {
            stripHumid.setPixelColor(pixel, 0);
        }
    }
    stripHumid.show();
}

// Zeigt die Uhrzeit auf dem TM1637 an
void displayTime(DateTime now) {
    int displayValue = now.hour() * 100 + now.minute();

    // Doppelpunkt blinkt jede Sekunde
    uint8_t colonMask = colonOn ? 0b01000000 : 0b00000000;

    display.showNumberDecEx(displayValue, colonMask, true);
}

// ===== SETUP =====

void setup() {
    Serial.begin(9600);
    Serial.println("Klimastation P03 startet...");

    // TM1637 Display initialisieren
    display.setBrightness(0x0f);  // Maximale Helligkeit
    display.clear();

    // Test-Anzeige
    uint8_t loading[] = { SEG_D, SEG_D, SEG_D, SEG_D };
    display.setSegments(loading);

    // NeoPixel initialisieren
    stripTemp.begin();
    stripTemp.setBrightness(50);  // Nicht zu hell
    stripTemp.clear();
    stripTemp.show();

    stripHumid.begin();
    stripHumid.setBrightness(50);
    stripHumid.clear();
    stripHumid.show();

    // DHT22 initialisieren
    dht.begin();

    // RTC initialisieren
    Wire.begin();
    if (!rtc.begin()) {
        Serial.println("RTC nicht gefunden!");
        // Fehleranzeige auf Display
        uint8_t err[] = { SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,  // E
                          SEG_E | SEG_G,                          // r
                          SEG_E | SEG_G,                          // r
                          0 };
        display.setSegments(err);
        while (true);
    }

    if (!rtc.isrunning()) {
        Serial.println("RTC setze Zeit...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // Startanimation - alle LEDs kurz aufleuchten
    for (int i = 0; i < NUM_LEDS; i++) {
        int pixel = logicalToPhysical(i);
        stripTemp.setPixelColor(pixel, getFixedScaleColor(stripTemp, i));
        stripHumid.setPixelColor(pixel, getFixedScaleColor(stripHumid, i));
        stripTemp.show();
        stripHumid.show();
        delay(100);
    }
    delay(500);
    stripTemp.clear();
    stripHumid.clear();
    stripTemp.show();
    stripHumid.show();

    Serial.println("Initialisierung abgeschlossen!");
}

// ===== HAUPTSCHLEIFE =====

void loop() {
    // Aktuelle Zeit holen
    DateTime now = rtc.now();

    // Uhrzeit anzeigen
    displayTime(now);

    // Doppelpunkt-Status wechseln (für Blinken)
    static unsigned long lastColonToggle = 0;
    if (millis() - lastColonToggle > 500) {
        colonOn = !colonOn;
        lastColonToggle = millis();
    }

    // Sensoren regelmäßig auslesen
    if (millis() - lastSensorRead > SENSOR_INTERVAL) {
        lastSensorRead = millis();

        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (!isnan(temperature) && !isnan(humidity)) {
            // Werte gültig - LEDs aktualisieren
            updateTemperatureLeds(temperature);
            updateHumidityLeds(humidity);

            // Debug-Ausgabe
            Serial.print("Temp: ");
            Serial.print(temperature);
            Serial.print(" C, Humidity: ");
            Serial.print(humidity);
            Serial.println(" %");
        } else {
            Serial.println("Fehler beim Lesen des DHT22!");
        }
    }

    delay(100);
}
