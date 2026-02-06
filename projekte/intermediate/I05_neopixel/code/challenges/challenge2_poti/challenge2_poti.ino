// I05 Challenge 2: Potentiometer steuert Farbton
// Drehe am Poti um durch den Regenbogen zu wandern

#include <FastLED.h>

#define DATA_PIN 7
#define NUM_LEDS 8
#define POTI_PIN A0

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(150);

  Serial.println("Challenge 2: Poti-Farbsteuerung");
  Serial.println("--------------------------------");
  Serial.println("Drehe am Potentiometer!");
}

void loop() {
  // Poti-Wert lesen (0-1023)
  int potiWert = analogRead(POTI_PIN);

  // Wert auf Hue-Bereich mappen (0-255)
  // 0 = Rot, 85 = Gruen, 170 = Blau, 255 = Rot
  int hue = map(potiWert, 0, 1023, 0, 255);

  // Alle LEDs auf die gleiche Farbe setzen
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);  // Volle Saettigung und Helligkeit
  }

  FastLED.show();

  // Debug-Ausgabe
  Serial.print("Poti: ");
  Serial.print(potiWert);
  Serial.print(" -> Hue: ");
  Serial.println(hue);

  delay(50);  // Kleine Pause fuer stabiles Lesen
}
