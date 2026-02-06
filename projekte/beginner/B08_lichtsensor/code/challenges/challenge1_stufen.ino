// Challenge 1: Helligkeitsstufen mit 3 LEDs
// Zeigt 3 Stufen: dunkel (3 LEDs), mittel (2 LEDs), hell (1 LED)

const int LDR_PIN = A0;       // Lichtsensor an A0
const int LED_GRUEN = 9;      // Gruen = hell (PWM)
const int LED_GELB = 10;      // Gelb = mittel (PWM)
const int LED_ROT = 11;       // Rot = dunkel (PWM)

// Schwellwerte fuer die drei Stufen
const int SCHWELLE_MITTEL = 400;  // Ab hier: mittel dunkel
const int SCHWELLE_DUNKEL = 700;  // Ab hier: sehr dunkel

void setup() {
  pinMode(LED_GRUEN, OUTPUT);
  pinMode(LED_GELB, OUTPUT);
  pinMode(LED_ROT, OUTPUT);

  Serial.begin(9600);
  Serial.println("Lichtsensor Stufenanzeige gestartet!");
  Serial.println("Gruen = hell, Gelb = mittel, Rot = dunkel");
}

void loop() {
  // LDR auslesen (0 = hell, 1023 = dunkel)
  int wert = analogRead(LDR_PIN);

  // Alle LEDs zuerst aus
  digitalWrite(LED_GRUEN, LOW);
  digitalWrite(LED_GELB, LOW);
  digitalWrite(LED_ROT, LOW);

  // Stufe bestimmen und LEDs entsprechend einschalten
  String stufe;

  if (wert < SCHWELLE_MITTEL) {
    // Hell: nur gruene LED an
    digitalWrite(LED_GRUEN, HIGH);
    stufe = "HELL (1 LED)";
  }
  else if (wert < SCHWELLE_DUNKEL) {
    // Mittel: gruene und gelbe LED an
    digitalWrite(LED_GRUEN, HIGH);
    digitalWrite(LED_GELB, HIGH);
    stufe = "MITTEL (2 LEDs)";
  }
  else {
    // Dunkel: alle drei LEDs an
    digitalWrite(LED_GRUEN, HIGH);
    digitalWrite(LED_GELB, HIGH);
    digitalWrite(LED_ROT, HIGH);
    stufe = "DUNKEL (3 LEDs)";
  }

  // Ausgabe am Serial Monitor
  Serial.print("LDR: ");
  Serial.print(wert);
  Serial.print(" | Stufe: ");
  Serial.println(stufe);

  delay(300);
}
