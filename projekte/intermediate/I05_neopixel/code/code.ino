// I05: WS2812 NeoPixel RGB-LED-Strip
// Adressierbare LEDs mit Lichteffekten

#include <FastLED.h>

// NeoPixel-Strip an Pin 7 mit 8 LEDs
#define DATA_PIN 7
#define NUM_LEDS 8

CRGB leds[NUM_LEDS];

void setup() {
    Serial.begin(9600);
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(128);

    Serial.println("WS2812 NeoPixel RGB-LEDs");
    Serial.println("------------------------------");
}

void clearAll() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void setAll(CRGB color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = color;
    }
    FastLED.show();
}

void colorWipe(CRGB color, int wait) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = color;
        FastLED.show();
        delay(wait);
    }
}

void runningLight(CRGB color, int wait) {
    for (int i = 0; i < NUM_LEDS; i++) {
        clearAll();
        leds[i] = color;
        FastLED.show();
        delay(wait);
    }
}

void blink(CRGB color, int times, int wait) {
    for (int t = 0; t < times; t++) {
        setAll(color);
        delay(wait);
        clearAll();
        delay(wait);
    }
}

void rainbowCycle(int wait) {
    for (int j = 0; j < 256; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            int pos = (i * 256 / NUM_LEDS + j) & 255;
            if (pos < 85) {
                leds[i] = CRGB(255 - pos * 3, pos * 3, 0);
            } else if (pos < 170) {
                pos -= 85;
                leds[i] = CRGB(0, 255 - pos * 3, pos * 3);
            } else {
                pos -= 170;
                leds[i] = CRGB(pos * 3, 0, 255 - pos * 3);
            }
        }
        FastLED.show();
        delay(wait);
    }
}

void loop() {
    Serial.println("  Rot Wipe");
    colorWipe(CRGB::Red, 100);
    delay(500);

    Serial.println("  Gruen Wipe");
    colorWipe(CRGB::Green, 100);
    delay(500);

    Serial.println("  Blau Wipe");
    colorWipe(CRGB::Blue, 100);
    delay(500);

    Serial.println("  Lauflicht");
    runningLight(CRGB::Red, 100);
    runningLight(CRGB::Green, 100);
    runningLight(CRGB::Blue, 100);
    runningLight(CRGB::Yellow, 100);

    Serial.println("  Blinken");
    blink(CRGB::Magenta, 3, 300);

    Serial.println("  Regenbogen");
    rainbowCycle(20);

    clearAll();
    delay(1000);
}
