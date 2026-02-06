// I05 Challenge 4: Reaktionsspiel
// Druecke den Taster wenn die gruene LED (Ziel) leuchtet!

#include <FastLED.h>

#define DATA_PIN 7
#define NUM_LEDS 8
#define TASTER_PIN 2

CRGB leds[NUM_LEDS];

// Spielvariablen
int zielLED = 3;            // Die LED die getroffen werden muss (gruene LED)
int aktuelleLED = 0;        // Aktuelle Position des Lauflichts
int geschwindigkeit = 150;  // Startgeschwindigkeit in ms
int punkte = 0;             // Punktestand
bool spielLaeuft = true;

// Taster-Entprellung
unsigned long letzterDruck = 0;
const int entprellZeit = 200;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(150);

  pinMode(TASTER_PIN, INPUT_PULLUP);

  // Zufallsgenerator initialisieren
  randomSeed(analogRead(A0));

  Serial.println("==============================");
  Serial.println("  REAKTIONSSPIEL");
  Serial.println("==============================");
  Serial.println("Druecke den Taster wenn das");
  Serial.println("Lauflicht auf der GRUENEN LED ist!");
  Serial.println("");
  Serial.println("Start in 3 Sekunden...");

  // Countdown anzeigen
  for (int i = 3; i > 0; i--) {
    alleAus();
    leds[i] = CRGB::Yellow;
    FastLED.show();
    delay(1000);
  }
  alleAus();
  FastLED.show();

  // Ziel-LED zufaellig waehlen
  neuesZiel();
}

void alleAus() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}

void neuesZiel() {
  zielLED = random(0, NUM_LEDS);
  Serial.print("Neues Ziel: LED ");
  Serial.println(zielLED);
}

void zeigeErgebnis(bool treffer) {
  alleAus();

  if (treffer) {
    // Gruenes Blinken bei Treffer
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = CRGB::Green;
      }
      FastLED.show();
      delay(100);
      alleAus();
      FastLED.show();
      delay(100);
    }
  } else {
    // Rotes Blinken bei Fehler
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = CRGB::Red;
      }
      FastLED.show();
      delay(100);
      alleAus();
      FastLED.show();
      delay(100);
    }
  }
}

void loop() {
  if (!spielLaeuft) {
    // Spiel beendet - warte auf Neustart
    if (digitalRead(TASTER_PIN) == LOW) {
      delay(200);
      punkte = 0;
      geschwindigkeit = 150;
      spielLaeuft = true;
      neuesZiel();
      Serial.println("\nNeues Spiel!");
    }
    return;
  }

  // Lauflicht anzeigen
  alleAus();

  // Ziel-LED immer gruen anzeigen (als Markierung)
  leds[zielLED] = CRGB::Green;
  leds[zielLED].fadeToBlackBy(180);  // Gedimmt als Hintergrund

  // Aktuelle Position hell anzeigen
  if (aktuelleLED == zielLED) {
    leds[aktuelleLED] = CRGB::Green;  // Volle Helligkeit wenn auf Ziel
  } else {
    leds[aktuelleLED] = CRGB::Blue;   // Blau wenn nicht auf Ziel
  }

  FastLED.show();

  // Taster pruefen
  if (digitalRead(TASTER_PIN) == LOW && millis() - letzterDruck > entprellZeit) {
    letzterDruck = millis();

    if (aktuelleLED == zielLED) {
      // TREFFER!
      punkte++;
      Serial.print("TREFFER! Punkte: ");
      Serial.println(punkte);

      zeigeErgebnis(true);

      // Spiel wird schneller
      if (geschwindigkeit > 50) {
        geschwindigkeit -= 10;
      }

      // Neues Ziel
      neuesZiel();

    } else {
      // DANEBEN!
      Serial.print("Daneben! Endstand: ");
      Serial.print(punkte);
      Serial.println(" Punkte");
      Serial.println("Druecke Taster fuer neues Spiel");

      zeigeErgebnis(false);
      spielLaeuft = false;
    }
  }

  delay(geschwindigkeit);

  // Naechste LED
  aktuelleLED++;
  if (aktuelleLED >= NUM_LEDS) {
    aktuelleLED = 0;
  }
}
