// B08: Lichtsensor (LDR) - Fertige Version
// Automatische Nachtlampe

const int LDR_PIN = A0;   // Lichtsensor an A0
const int LED_PIN = 9;    // LED mit PWM an Pin 9

// Schwellwert fuer "dunkel" (0-1023, anpassen je nach Umgebung)
const int DUNKEL_SCHWELLE = 600;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Lichtsensor gestartet!");
  Serial.println("Bei Dunkelheit leuchtet die LED.");
  Serial.println("In Wokwi: Klicke auf den LDR um Helligkeit zu aendern.");
}

void loop() {
  // LDR auslesen (0-1023)
  int wert = analogRead(LDR_PIN);

  // Status bestimmen
  String status;
  int ledHelligkeit;

  if (wert > DUNKEL_SCHWELLE) {
    // Dunkel: LED proportional zur Dunkelheit einschalten
    ledHelligkeit = map(wert, DUNKEL_SCHWELLE, 1023, 0, 255);
    ledHelligkeit = constrain(ledHelligkeit, 0, 255);
    status = "DUNKEL";
  } else {
    // Hell: LED aus
    ledHelligkeit = 0;
    status = "HELL";
  }

  analogWrite(LED_PIN, ledHelligkeit);

  // Prozent berechnen (0% = hell, 100% = dunkel)
  int dunkelheit = wert * 100 / 1023;

  Serial.print("LDR: ");
  Serial.print(wert);
  Serial.print(" | Dunkelheit: ");
  Serial.print(dunkelheit);
  Serial.print("% | ");
  Serial.println(status);

  delay(300);
}
