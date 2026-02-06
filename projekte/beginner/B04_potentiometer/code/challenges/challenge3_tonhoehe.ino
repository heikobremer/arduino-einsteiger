// B04 Challenge 3: Tonhöhe mit Poti steuern
// Piezo spielt Ton dessen Frequenz vom Poti abhaengt

const int POTI_PIN = A0;
const int BUZZER_PIN = 9;

// Frequenzbereich (C4 bis C6)
const int FREQ_MIN = 262;   // C4
const int FREQ_MAX = 1047;  // C6

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Theremin - Drehe am Poti fuer verschiedene Toene!");
}

void loop() {
  int wert = analogRead(POTI_PIN);  // 0-1023

  if (wert < 10) {
    // Ganz links = Stille
    noTone(BUZZER_PIN);
    Serial.println("Stille");
  } else {
    // Frequenz berechnen
    int frequenz = map(wert, 10, 1023, FREQ_MIN, FREQ_MAX);
    tone(BUZZER_PIN, frequenz);

    Serial.print("Frequenz: ");
    Serial.print(frequenz);
    Serial.println(" Hz");
  }

  delay(50);
}
