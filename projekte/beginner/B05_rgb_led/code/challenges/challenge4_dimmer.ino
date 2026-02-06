// Challenge 4: Ein-Poti Farbsteuerung
// Ein einzelnes Potentiometer steuert alle drei Farben nacheinander:
// 0-341:     Rot (Intensitaet von 0 bis 255)
// 342-682:   Gruen (Intensitaet von 0 bis 255)
// 683-1023:  Blau (Intensitaet von 0 bis 255)

const int POTI = A0;    // Potentiometer an Analog-Pin A0

const int LED_R = 9;    // Rote LED an PWM-Pin 9
const int LED_G = 10;   // Gruene LED an PWM-Pin 10
const int LED_B = 11;   // Blaue LED an PWM-Pin 11

// Grenzen fuer die drei Farbbereiche
const int GRENZE_ROT = 341;     // 0 bis 341 = Rot
const int GRENZE_GRUEN = 682;   // 342 bis 682 = Gruen
// 683 bis 1023 = Blau

void setup() {
  // RGB-LED Pins als Ausgang
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  Serial.begin(9600);
  Serial.println("Ein-Poti Farbsteuerung gestartet!");
  Serial.println("0-341: Rot | 342-682: Gruen | 683-1023: Blau");
}

void loop() {
  // Poti-Wert lesen (0-1023)
  int potiWert = analogRead(POTI);

  // Farben zuruecksetzen
  int r = 0;
  int g = 0;
  int b = 0;

  // Bestimme welche Farbe aktiv ist
  if (potiWert <= GRENZE_ROT) {
    // Rot-Bereich: 0-341
    // Mappe auf 0-255 fuer Helligkeit
    r = map(potiWert, 0, GRENZE_ROT, 0, 255);
    Serial.print("ROT - ");
  }
  else if (potiWert <= GRENZE_GRUEN) {
    // Gruen-Bereich: 342-682
    // Mappe auf 0-255 fuer Helligkeit
    g = map(potiWert, GRENZE_ROT + 1, GRENZE_GRUEN, 0, 255);
    Serial.print("GRUEN - ");
  }
  else {
    // Blau-Bereich: 683-1023
    // Mappe auf 0-255 fuer Helligkeit
    b = map(potiWert, GRENZE_GRUEN + 1, 1023, 0, 255);
    Serial.print("BLAU - ");
  }

  // RGB-LED steuern
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);

  // Werte anzeigen
  Serial.print("Poti: ");
  Serial.print(potiWert);
  Serial.print(" | R:");
  Serial.print(r);
  Serial.print(" G:");
  Serial.print(g);
  Serial.print(" B:");
  Serial.println(b);

  delay(100);
}
