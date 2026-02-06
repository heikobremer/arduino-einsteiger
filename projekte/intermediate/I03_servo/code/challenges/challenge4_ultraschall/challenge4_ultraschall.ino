// I03 Challenge 4: Servo als Radar mit Ultraschall
// Servo schwenkt und misst Entfernung bei verschiedenen Winkeln

#include <Servo.h>

#define SERVO_PIN 9
#define TRIG_PIN 7      // Ultraschall Trigger
#define ECHO_PIN 6      // Ultraschall Echo

Servo servo;

// Radar-Konfiguration
int startWinkel = 15;     // Startwinkel (nicht ganz links)
int endWinkel = 165;      // Endwinkel (nicht ganz rechts)
int schrittweite = 15;    // Grad pro Messung
int messDelay = 100;      // Zeit fuer Servo-Bewegung

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO_PIN);

    // Ultraschall-Pins
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.println("Challenge 4: Ultraschall-Radar");
    Serial.println("Servo schwenkt und misst Entfernung");
    Serial.println("================================");

    // Startposition
    servo.write(startWinkel);
    delay(500);
}

void loop() {
    Serial.println("\n--- Neuer Scan ---");

    // Scan von links nach rechts
    for (int winkel = startWinkel; winkel <= endWinkel; winkel += schrittweite) {
        // Servo auf Winkel bewegen
        servo.write(winkel);
        delay(messDelay);  // Warten bis Servo angekommen

        // Entfernung messen
        float entfernung = messeEntfernung();

        // Ergebnis ausgeben
        Serial.print("Winkel: ");
        if (winkel < 100) Serial.print(" ");
        if (winkel < 10) Serial.print(" ");
        Serial.print(winkel);
        Serial.print(" Grad | Entfernung: ");

        if (entfernung > 0 && entfernung < 400) {
            Serial.print(entfernung, 1);
            Serial.print(" cm ");

            // Visuelle Darstellung mit Balken
            int balken = (int)(entfernung / 10);
            for (int i = 0; i < balken && i < 30; i++) {
                Serial.print("#");
            }
        } else {
            Serial.print("------- (kein Echo)");
        }
        Serial.println();
    }

    // Kurze Pause vor naechstem Scan
    delay(500);

    // Scan von rechts nach links
    for (int winkel = endWinkel; winkel >= startWinkel; winkel -= schrittweite) {
        servo.write(winkel);
        delay(messDelay);

        float entfernung = messeEntfernung();

        Serial.print("Winkel: ");
        if (winkel < 100) Serial.print(" ");
        if (winkel < 10) Serial.print(" ");
        Serial.print(winkel);
        Serial.print(" Grad | Entfernung: ");

        if (entfernung > 0 && entfernung < 400) {
            Serial.print(entfernung, 1);
            Serial.print(" cm ");

            int balken = (int)(entfernung / 10);
            for (int i = 0; i < balken && i < 30; i++) {
                Serial.print("#");
            }
        } else {
            Serial.print("------- (kein Echo)");
        }
        Serial.println();
    }

    delay(1000);
}

// Funktion: Ultraschall-Entfernung messen
float messeEntfernung() {
    // Trigger-Puls senden (10 Mikrosekunden)
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Echo-Zeit messen (Timeout nach 30ms = ca. 5m)
    long dauer = pulseIn(ECHO_PIN, HIGH, 30000);

    // Keine Antwort?
    if (dauer == 0) {
        return -1;
    }

    // Entfernung berechnen: Schallgeschwindigkeit 343 m/s
    // dauer in Mikrosekunden, hin und zurueck -> /2
    // 343 m/s = 0.0343 cm/us -> entfernung = dauer * 0.0343 / 2
    float entfernung = dauer * 0.0171;

    return entfernung;
}
