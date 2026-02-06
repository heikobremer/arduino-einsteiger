// I05 Challenge 3: Knight Rider Lauflicht
// Ein Licht wandert hin und her wie bei K.I.T.T.

#include <FastLED.h>

#define DATA_PIN 7
#define NUM_LEDS 8

CRGB leds[NUM_LEDS];

// Farbe des Lauflichts (klassisch Rot)
CRGB hauptfarbe = CRGB::Red;

// Geschwindigkeit in Millisekunden
int geschwindigkeit = 80;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(200);

  Serial.println("Challenge 3: Knight Rider");
  Serial.println("--------------------------");
}

// Alle LEDs ausschalten
void alleAus() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}

// Knight Rider Effekt mit Schweif
void knightRider() {
  // Vorwaerts (links nach rechts)
  for (int i = 0; i < NUM_LEDS; i++) {
    alleAus();

    // Hauptlicht
    leds[i] = hauptfarbe;

    // Schweif (nachleuchtende LEDs mit abnehmender Helligkeit)
    if (i > 0) {
      leds[i-1] = hauptfarbe;
      leds[i-1].fadeToBlackBy(150);  // 60% dunkler
    }
    if (i > 1) {
      leds[i-2] = hauptfarbe;
      leds[i-2].fadeToBlackBy(220);  // 85% dunkler
    }

    FastLED.show();
    delay(geschwindigkeit);
  }

  // Rueckwaerts (rechts nach links)
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    alleAus();

    // Hauptlicht
    leds[i] = hauptfarbe;

    // Schweif in umgekehrter Richtung
    if (i < NUM_LEDS - 1) {
      leds[i+1] = hauptfarbe;
      leds[i+1].fadeToBlackBy(150);
    }
    if (i < NUM_LEDS - 2) {
      leds[i+2] = hauptfarbe;
      leds[i+2].fadeToBlackBy(220);
    }

    FastLED.show();
    delay(geschwindigkeit);
  }
}

void loop() {
  knightRider();
}
