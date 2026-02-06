// Challenge 1: LED-Bargraph Entfernungsanzeige
// 5 LEDs zeigen die Entfernung von 0-50cm an
// Je naeher das Objekt, desto mehr LEDs leuchten

#define TRIGGER_PIN 6
#define ECHO_PIN 7

// 5 LEDs fuer Bargraph-Anzeige
const int LED_PINS[] = {8, 9, 10, 11, 12};
const int NUM_LEDS = 5;

// Entfernungsbereiche (in cm)
const int MAX_DISTANCE = 50;  // Maximale angezeigte Entfernung

void setup() {
    Serial.begin(9600);

    // Ultraschall-Sensor Pins
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // LED Pins als Ausgaenge
    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(LED_PINS[i], OUTPUT);
    }

    Serial.println("LED-Bargraph Entfernungsanzeige");
    Serial.println("0-50cm -> 5 LEDs");
}

float measureDistance() {
    // Trigger-Signal senden (10us High-Puls)
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Echo-Puls messen (Timeout: 30ms)
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0) {
        return -1;  // Timeout oder Fehler
    }

    // Entfernung berechnen
    float distance = (duration / 2.0) / 29.1;
    return distance;
}

void updateBargraph(float distance) {
    // Anzahl der LEDs berechnen (invertiert: naeher = mehr LEDs)
    int ledsOn = 0;

    if (distance > 0 && distance <= MAX_DISTANCE) {
        // Bereich 0-50cm auf 5 LEDs abbilden
        // 0cm = 5 LEDs, 50cm = 0 LEDs
        ledsOn = map(MAX_DISTANCE - (int)distance, 0, MAX_DISTANCE, 0, NUM_LEDS);
        ledsOn = constrain(ledsOn, 0, NUM_LEDS);
    } else if (distance > MAX_DISTANCE) {
        ledsOn = 0;  // Zu weit weg
    }

    // LEDs entsprechend schalten
    for (int i = 0; i < NUM_LEDS; i++) {
        if (i < ledsOn) {
            digitalWrite(LED_PINS[i], HIGH);
        } else {
            digitalWrite(LED_PINS[i], LOW);
        }
    }
}

void loop() {
    float dist = measureDistance();

    if (dist > 0) {
        Serial.print("Entfernung: ");
        Serial.print(dist, 1);
        Serial.print(" cm -> ");

        // Bargraph aktualisieren
        updateBargraph(dist);

        // Anzahl LEDs anzeigen
        int ledsOn = 0;
        if (dist <= MAX_DISTANCE) {
            ledsOn = map(MAX_DISTANCE - (int)dist, 0, MAX_DISTANCE, 0, NUM_LEDS);
            ledsOn = constrain(ledsOn, 0, NUM_LEDS);
        }
        Serial.print(ledsOn);
        Serial.println(" LEDs an");
    } else {
        Serial.println("Messfehler");
        // Alle LEDs aus bei Fehler
        for (int i = 0; i < NUM_LEDS; i++) {
            digitalWrite(LED_PINS[i], LOW);
        }
    }

    delay(100);  // Schnelle Aktualisierung
}
