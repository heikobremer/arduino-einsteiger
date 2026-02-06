// Challenge 4: Glaettung durch Durchschnittsberechnung
// Speichert die letzten 5 Messungen und bildet Durchschnitt
// Reduziert Messrauschen fuer stabilere Werte

#define TRIGGER_PIN 6
#define ECHO_PIN 7

// Anzahl der Messungen fuer Durchschnitt
const int NUM_READINGS = 5;

// Array fuer die letzten Messungen
float readings[NUM_READINGS];
int readIndex = 0;        // Aktueller Index im Array
float total = 0;          // Summe aller Werte
float average = 0;        // Berechneter Durchschnitt
bool bufferFilled = false; // Wurde Buffer mindestens einmal gefuellt?

void setup() {
    Serial.begin(9600);

    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Array initialisieren
    for (int i = 0; i < NUM_READINGS; i++) {
        readings[i] = 0;
    }

    Serial.println("Ultraschall mit Durchschnittsberechnung");
    Serial.println("Mittelt die letzten 5 Messungen");
    Serial.println("----------------------------------------");
}

float measureDistance() {
    // Trigger-Signal senden
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Echo messen
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0) {
        return -1;
    }

    return (duration / 2.0) / 29.1;
}

float updateAverage(float newReading) {
    // Nur gueltige Messungen verarbeiten
    if (newReading < 0) {
        return average;  // Bei Fehler alten Durchschnitt behalten
    }

    // Alten Wert von Summe abziehen
    total = total - readings[readIndex];

    // Neuen Wert speichern und zur Summe addieren
    readings[readIndex] = newReading;
    total = total + newReading;

    // Index weiterschalten (Ringpuffer)
    readIndex = (readIndex + 1) % NUM_READINGS;

    // Pruefen ob Buffer einmal gefuellt wurde
    if (readIndex == 0) {
        bufferFilled = true;
    }

    // Durchschnitt berechnen
    if (bufferFilled) {
        average = total / NUM_READINGS;
    } else {
        // Waehrend Fuellphase: nur durch aktuelle Anzahl teilen
        average = total / (readIndex > 0 ? readIndex : 1);
    }

    return average;
}

void loop() {
    float rawDistance = measureDistance();

    // Durchschnitt aktualisieren
    float smoothedDistance = updateAverage(rawDistance);

    // Ausgabe
    Serial.print("Roh: ");
    if (rawDistance > 0) {
        Serial.print(rawDistance, 1);
        Serial.print(" cm");
    } else {
        Serial.print("Fehler");
    }

    Serial.print(" | Durchschnitt: ");
    Serial.print(smoothedDistance, 1);
    Serial.print(" cm");

    // Differenz anzeigen (zeigt wie stark Glaettung wirkt)
    if (rawDistance > 0) {
        float diff = rawDistance - smoothedDistance;
        Serial.print(" | Diff: ");
        if (diff >= 0) Serial.print("+");
        Serial.print(diff, 1);
    }

    Serial.println();

    // Debug: Buffer-Inhalt anzeigen (optional)
    Serial.print("   Buffer: [");
    for (int i = 0; i < NUM_READINGS; i++) {
        Serial.print(readings[i], 0);
        if (i < NUM_READINGS - 1) Serial.print(", ");
    }
    Serial.println("]");

    delay(300);
}
