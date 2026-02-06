// Challenge 4: Helligkeits-Speicher
// Merkt sich den hellsten und dunkelsten Wert

const int LDR_PIN = A0;       // Lichtsensor an A0
const int LED_PIN = 9;        // LED zur Anzeige

// Gespeicherte Extremwerte
int hellsterWert = 1023;      // Startwert: Maximum (wird kleiner)
int dunkelsterWert = 0;       // Startwert: Minimum (wird groesser)

// Anzahl der Messungen
unsigned long messungen = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("=================================");
  Serial.println("  Helligkeits-Speicher gestartet");
  Serial.println("=================================");
  Serial.println("");
  Serial.println("Bewege die Helligkeit am LDR.");
  Serial.println("Die Extremwerte werden gespeichert!");
  Serial.println("");
}

void loop() {
  // LDR auslesen
  int wert = analogRead(LDR_PIN);
  messungen++;

  // Pruefen ob neuer Rekord
  bool neuerHellster = false;
  bool neuerDunkelster = false;

  if (wert < hellsterWert) {
    hellsterWert = wert;
    neuerHellster = true;
  }

  if (wert > dunkelsterWert) {
    dunkelsterWert = wert;
    neuerDunkelster = true;
  }

  // LED zeigt aktuellen Wert relativ zu den Extremen
  int spanne = dunkelsterWert - hellsterWert;
  int ledHelligkeit = 0;

  if (spanne > 10) {
    // LED-Helligkeit basierend auf gemessenem Bereich
    ledHelligkeit = map(wert, hellsterWert, dunkelsterWert, 0, 255);
    ledHelligkeit = constrain(ledHelligkeit, 0, 255);
  }

  analogWrite(LED_PIN, ledHelligkeit);

  // Ausgabe bei neuem Rekord
  if (neuerHellster) {
    Serial.println("");
    Serial.print("*** NEUER HELLSTER WERT: ");
    Serial.print(hellsterWert);
    Serial.println(" ***");
  }

  if (neuerDunkelster) {
    Serial.println("");
    Serial.print("*** NEUER DUNKELSTER WERT: ");
    Serial.print(dunkelsterWert);
    Serial.println(" ***");
  }

  // Normale Statusausgabe (alle 10 Messungen)
  if (messungen % 10 == 0) {
    Serial.println("---------------------------------");
    Serial.print("Messung #");
    Serial.println(messungen);
    Serial.print("Aktuell:   ");
    Serial.println(wert);
    Serial.print("Hellster:  ");
    Serial.print(hellsterWert);
    Serial.println(" (niedrigster Wert)");
    Serial.print("Dunkelster: ");
    Serial.print(dunkelsterWert);
    Serial.println(" (hoechster Wert)");
    Serial.print("Spanne:    ");
    Serial.println(spanne);
  }

  delay(200);
}
