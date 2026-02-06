// Challenge 2: Nachtlicht mit sanftem Fading
// LED geht langsam an und aus statt abrupt

const int LDR_PIN = A0;       // Lichtsensor an A0
const int LED_PIN = 9;        // LED mit PWM an Pin 9

// Schwellwert fuer "dunkel"
const int DUNKEL_SCHWELLE = 600;

// Aktuelle LED-Helligkeit (merken fuer sanftes Fading)
int aktuelleHelligkeit = 0;

// Fade-Geschwindigkeit (hoeher = schneller)
const int FADE_SCHRITT = 5;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Nachtlicht mit Fading gestartet!");
  Serial.println("LED geht sanft an und aus.");
}

void loop() {
  // LDR auslesen
  int wert = analogRead(LDR_PIN);

  // Ziel-Helligkeit berechnen
  int zielHelligkeit;

  if (wert > DUNKEL_SCHWELLE) {
    // Dunkel: LED soll leuchten (proportional zur Dunkelheit)
    zielHelligkeit = map(wert, DUNKEL_SCHWELLE, 1023, 50, 255);
    zielHelligkeit = constrain(zielHelligkeit, 0, 255);
  } else {
    // Hell: LED soll aus sein
    zielHelligkeit = 0;
  }

  // Sanft zur Ziel-Helligkeit faden
  if (aktuelleHelligkeit < zielHelligkeit) {
    // LED muss heller werden
    aktuelleHelligkeit += FADE_SCHRITT;
    if (aktuelleHelligkeit > zielHelligkeit) {
      aktuelleHelligkeit = zielHelligkeit;
    }
  }
  else if (aktuelleHelligkeit > zielHelligkeit) {
    // LED muss dunkler werden
    aktuelleHelligkeit -= FADE_SCHRITT;
    if (aktuelleHelligkeit < zielHelligkeit) {
      aktuelleHelligkeit = zielHelligkeit;
    }
  }

  // LED mit aktueller Helligkeit ansteuern
  analogWrite(LED_PIN, aktuelleHelligkeit);

  // Status ausgeben
  Serial.print("LDR: ");
  Serial.print(wert);
  Serial.print(" | Ziel: ");
  Serial.print(zielHelligkeit);
  Serial.print(" | Aktuell: ");
  Serial.println(aktuelleHelligkeit);

  // Kurze Pause fuer sanften Uebergang
  delay(30);
}
