// I02: HC-SR04 Ultraschall-Entfernungsmesser
// Entfernung messen mit Schallwellen

// Trigger an Pin 6, Echo an Pin 7
#define TRIGGER_PIN 6
#define ECHO_PIN 7

void setup() {
    Serial.begin(9600);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.println("HC-SR04 Ultraschall-Sensor");
    Serial.println("------------------------------");
}

float measureDistance() {
    // Trigger-Signal senden (10us High-Puls)
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Echo-Puls messen (Timeout: 30ms = ca. 5m)
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0) {
        return -1;  // Timeout oder Fehler
    }

    // Entfernung berechnen: Schallgeschwindigkeit 343 m/s
    // Zeit ist hin und zurueck, also durch 2
    // 343 m/s = 0.0343 cm/us -> 29.1 us/cm
    float distance = (duration / 2.0) / 29.1;

    return distance;
}

void loop() {
    float dist = measureDistance();

    if (dist > 0) {
        Serial.print("Entfernung: ");
        Serial.print(dist, 1);
        Serial.println(" cm");

        // Warnungen bei bestimmten Entfernungen
        if (dist < 10) {
            Serial.println("  -> Sehr nah!");
        } else if (dist < 30) {
            Serial.println("  -> Nah");
        }
    } else {
        Serial.println("Messfehler oder ausser Reichweite");
    }

    delay(500);
}
