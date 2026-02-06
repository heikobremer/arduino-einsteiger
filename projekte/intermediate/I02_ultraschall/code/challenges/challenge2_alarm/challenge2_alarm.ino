// Challenge 2: Entfernungs-Alarm mit Buzzer
// Buzzer piept schneller, je naeher das Objekt ist
// Wie ein Einparksensor im Auto

#define TRIGGER_PIN 6
#define ECHO_PIN 7
#define BUZZER_PIN 8

// Entfernungsbereiche fuer Piep-Intervalle
const int ALARM_RANGE = 100;    // Maximale Alarmreichweite in cm
const int MIN_DELAY = 50;       // Kuerzeste Pause (sehr nah)
const int MAX_DELAY = 500;      // Laengste Pause (weit weg)
const int BEEP_DURATION = 50;   // Dauer eines Piep-Tons

unsigned long lastBeepTime = 0;
int currentDelay = MAX_DELAY;

void setup() {
    Serial.begin(9600);

    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    Serial.println("Ultraschall-Alarm");
    Serial.println("Naeher = schnelleres Piepen");
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

void updateAlarm(float distance) {
    if (distance < 0 || distance > ALARM_RANGE) {
        // Ausserhalb Reichweite - kein Alarm
        currentDelay = 0;  // Kein Piepen
        digitalWrite(BUZZER_PIN, LOW);
        return;
    }

    // Piep-Intervall basierend auf Entfernung berechnen
    // Naeher = kuerzeres Intervall
    currentDelay = map((int)distance, 0, ALARM_RANGE, MIN_DELAY, MAX_DELAY);
    currentDelay = constrain(currentDelay, MIN_DELAY, MAX_DELAY);

    // Dauerton bei sehr nahen Objekten (unter 5cm)
    if (distance < 5) {
        digitalWrite(BUZZER_PIN, HIGH);
        return;
    }

    // Piep-Logik mit Millis (nicht blockierend)
    unsigned long currentTime = millis();

    if (currentTime - lastBeepTime >= currentDelay) {
        // Kurzer Piep-Ton
        digitalWrite(BUZZER_PIN, HIGH);
        delay(BEEP_DURATION);
        digitalWrite(BUZZER_PIN, LOW);
        lastBeepTime = currentTime;
    }
}

void loop() {
    float dist = measureDistance();

    if (dist > 0) {
        Serial.print("Entfernung: ");
        Serial.print(dist, 1);
        Serial.print(" cm");

        if (dist <= ALARM_RANGE) {
            Serial.print(" -> Intervall: ");
            Serial.print(currentDelay);
            Serial.println(" ms");
        } else {
            Serial.println(" -> Kein Alarm");
        }

        updateAlarm(dist);
    } else {
        Serial.println("Messfehler");
        digitalWrite(BUZZER_PIN, LOW);
    }

    delay(50);  // Kurze Pause fuer schnelle Reaktion
}
