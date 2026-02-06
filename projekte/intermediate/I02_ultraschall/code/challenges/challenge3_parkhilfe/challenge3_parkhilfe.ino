// Challenge 3: Einparkhilfe mit 3 LEDs
// Gruen = sicher (>50cm), Gelb = Vorsicht (20-50cm), Rot = Stopp (<20cm)
// Simuliert eine echte Einparkhilfe

#define TRIGGER_PIN 6
#define ECHO_PIN 7

// LED Pins (wie Ampel)
#define LED_GRUEN 8
#define LED_GELB 9
#define LED_ROT 10

// Entfernungsschwellen in cm
const int GRENZE_GRUEN = 50;   // Ueber 50cm = sicher
const int GRENZE_GELB = 20;    // 20-50cm = Vorsicht
// Unter 20cm = Stopp (Rot)

void setup() {
    Serial.begin(9600);

    // Ultraschall-Sensor
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // LEDs als Ausgaenge
    pinMode(LED_GRUEN, OUTPUT);
    pinMode(LED_GELB, OUTPUT);
    pinMode(LED_ROT, OUTPUT);

    // Alle LEDs aus
    digitalWrite(LED_GRUEN, LOW);
    digitalWrite(LED_GELB, LOW);
    digitalWrite(LED_ROT, LOW);

    Serial.println("Einparkhilfe gestartet");
    Serial.println("Gruen >50cm | Gelb 20-50cm | Rot <20cm");
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

void updateParkingLights(float distance) {
    // Alle LEDs zunaechst aus
    digitalWrite(LED_GRUEN, LOW);
    digitalWrite(LED_GELB, LOW);
    digitalWrite(LED_ROT, LOW);

    if (distance < 0) {
        // Messfehler - rote LED blinkt
        digitalWrite(LED_ROT, HIGH);
        delay(100);
        digitalWrite(LED_ROT, LOW);
        return;
    }

    // Entfernung auswerten und passende LED aktivieren
    if (distance > GRENZE_GRUEN) {
        // Sicher - Gruen
        digitalWrite(LED_GRUEN, HIGH);
        Serial.println("  -> GRUEN: Weiterfahren");
    }
    else if (distance > GRENZE_GELB) {
        // Vorsicht - Gelb
        digitalWrite(LED_GELB, HIGH);
        Serial.println("  -> GELB: Langsam");
    }
    else if (distance > 5) {
        // Stopp - Rot
        digitalWrite(LED_ROT, HIGH);
        Serial.println("  -> ROT: Anhalten!");
    }
    else {
        // Sehr nah - Rot blinkt schnell
        digitalWrite(LED_ROT, HIGH);
        delay(100);
        digitalWrite(LED_ROT, LOW);
        delay(100);
        digitalWrite(LED_ROT, HIGH);
        Serial.println("  -> ROT BLINKT: ZU NAH!");
    }
}

void loop() {
    float dist = measureDistance();

    if (dist > 0) {
        Serial.print("Entfernung: ");
        Serial.print(dist, 1);
        Serial.println(" cm");

        updateParkingLights(dist);
    } else {
        Serial.println("Messfehler - ausser Reichweite");
        // Alle LEDs aus bei Fehler
        digitalWrite(LED_GRUEN, LOW);
        digitalWrite(LED_GELB, LOW);
        digitalWrite(LED_ROT, LOW);
    }

    delay(200);
}
