// I05 Challenge 1: Feuer-Effekt
// Simuliert flackerndes Feuer/Kerze mit zufaelligen Orange/Rot-Toenen

#include <FastLED.h>

#define DATA_PIN 7
#define NUM_LEDS 8

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(200);

  Serial.println("Challenge 1: Feuer-Effekt");
  Serial.println("---------------------------");

  // Zufallsgenerator initialisieren
  randomSeed(analogRead(A0));
}

void loop() {
  // Feuer-Effekt fuer jede LED
  for (int i = 0; i < NUM_LEDS; i++) {
    // Zufaellige Helligkeit fuer Flacker-Effekt (80-255)
    int helligkeit = random(80, 256);

    // Zufaellige Farbe zwischen Rot und Gelb
    // Hue 0 = Rot, Hue 32 = Orange, Hue 48 = Gelb
    int farbton = random(0, 48);

    // Saettigung variieren fuer mehr Tiefe
    int saettigung = random(200, 256);

    // Farbe setzen (HSV: Hue, Saturation, Value/Helligkeit)
    leds[i] = CHSV(farbton, saettigung, helligkeit);
  }

  FastLED.show();

  // Kurze, zufaellige Pause fuer realistisches Flackern
  delay(random(30, 100));
}
