// Challenge 3: Taster zum Durchschalten von Farben
// Mit jedem Tastendruck wechselt die RGB-LED zur naechsten Farbe

const int LED_R = 9;      // Rote LED an PWM-Pin 9
const int LED_G = 10;     // Gruene LED an PWM-Pin 10
const int LED_B = 11;     // Blaue LED an PWM-Pin 11
const int TASTER = 2;     // Taster an Pin 2

// Farbdefinitionen als Arrays [R, G, B]
const int FARBEN[][3] = {
  {255, 0, 0},      // Rot
  {255, 127, 0},    // Orange
  {255, 255, 0},    // Gelb
  {0, 255, 0},      // Gruen
  {0, 255, 255},    // Cyan
  {0, 0, 255},      // Blau
  {127, 0, 255},    // Violett
  {255, 0, 255},    // Magenta
  {255, 255, 255},  // Weiss
  {0, 0, 0}         // Aus
};

// Namen der Farben fuer Serial-Ausgabe
const char* FARB_NAMEN[] = {
  "Rot", "Orange", "Gelb", "Gruen", "Cyan",
  "Blau", "Violett", "Magenta", "Weiss", "Aus"
};

const int ANZAHL_FARBEN = 10;  // Anzahl der Farben im Array

int aktuelleFarbe = 0;         // Index der aktuellen Farbe
int letzterTasterStatus = HIGH; // Letzter Zustand des Tasters (Pull-up)

void setup() {
  // RGB-LED Pins als Ausgang
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // Taster als Eingang mit internem Pull-up Widerstand
  pinMode(TASTER, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Farbwechsel mit Taster gestartet!");
  Serial.println("Druecke den Taster zum Wechseln der Farbe.");

  // Startfarbe anzeigen
  zeigeFarbe();
}

void loop() {
  // Taster auslesen
  int tasterStatus = digitalRead(TASTER);

  // Pruefe ob Taster gedrueckt wurde (Flanke von HIGH zu LOW)
  if (tasterStatus == LOW && letzterTasterStatus == HIGH) {
    // Entprellen - kurz warten
    delay(50);

    // Zur naechsten Farbe wechseln
    aktuelleFarbe++;
    if (aktuelleFarbe >= ANZAHL_FARBEN) {
      aktuelleFarbe = 0;  // Zurueck zur ersten Farbe
    }

    // Neue Farbe anzeigen
    zeigeFarbe();
  }

  // Taster-Status speichern
  letzterTasterStatus = tasterStatus;

  // Kleine Verzoegerung
  delay(10);
}

// Hilfsfunktion: Aktuelle Farbe auf LED und Serial ausgeben
void zeigeFarbe() {
  // RGB-Werte setzen
  analogWrite(LED_R, FARBEN[aktuelleFarbe][0]);
  analogWrite(LED_G, FARBEN[aktuelleFarbe][1]);
  analogWrite(LED_B, FARBEN[aktuelleFarbe][2]);

  // Im Serial Monitor ausgeben
  Serial.print("Farbe ");
  Serial.print(aktuelleFarbe + 1);
  Serial.print("/");
  Serial.print(ANZAHL_FARBEN);
  Serial.print(": ");
  Serial.println(FARB_NAMEN[aktuelleFarbe]);
}
