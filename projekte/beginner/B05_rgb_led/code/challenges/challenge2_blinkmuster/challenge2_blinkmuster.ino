// Challenge 2: Blinkmuster mit vordefinierten Farben
// Die RGB-LED blinkt nacheinander in Rot, Gruen, Blau und Weiss

const int LED_R = 9;    // Rote LED an PWM-Pin 9
const int LED_G = 10;   // Gruene LED an PWM-Pin 10
const int LED_B = 11;   // Blaue LED an PWM-Pin 11

const int BLINK_DELAY = 500;  // Zeit pro Farbe in Millisekunden

// Farbdefinitionen als Arrays [R, G, B]
const int FARBEN[][3] = {
  {255, 0, 0},      // Rot
  {0, 255, 0},      // Gruen
  {0, 0, 255},      // Blau
  {255, 255, 255}   // Weiss
};

// Namen der Farben fuer Serial-Ausgabe
const char* FARB_NAMEN[] = {"Rot", "Gruen", "Blau", "Weiss"};

const int ANZAHL_FARBEN = 4;  // Anzahl der Farben im Array

int aktuelleFarbe = 0;  // Index der aktuellen Farbe

void setup() {
  // RGB-LED Pins als Ausgang
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  Serial.begin(9600);
  Serial.println("Blinkmuster gestartet!");
  Serial.println("Farbreihenfolge: Rot -> Gruen -> Blau -> Weiss");
}

void loop() {
  // Aktuelle Farbe setzen
  setFarbe(FARBEN[aktuelleFarbe][0],   // Rot-Wert
           FARBEN[aktuelleFarbe][1],   // Gruen-Wert
           FARBEN[aktuelleFarbe][2]);  // Blau-Wert

  // Farbe im Serial Monitor anzeigen
  Serial.print("Aktuelle Farbe: ");
  Serial.println(FARB_NAMEN[aktuelleFarbe]);

  // Warten
  delay(BLINK_DELAY);

  // LED aus (kurze Pause zwischen Farben)
  setFarbe(0, 0, 0);
  delay(100);

  // Zur naechsten Farbe wechseln
  aktuelleFarbe++;
  if (aktuelleFarbe >= ANZAHL_FARBEN) {
    aktuelleFarbe = 0;  // Zurueck zur ersten Farbe
    Serial.println("--- Zyklus neu gestartet ---");
  }
}

// Hilfsfunktion: RGB-Farbe setzen
void setFarbe(int r, int g, int b) {
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);
}
