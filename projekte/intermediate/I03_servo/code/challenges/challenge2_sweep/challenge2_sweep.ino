// I03 Challenge 2: Smooth Sweep - Sanftes Hin- und Herschwenken
// Mit einstellbarer Geschwindigkeit fuer fluessige Bewegung

#include <Servo.h>

#define SERVO_PIN 9

Servo servo;

// Konfiguration - hier Geschwindigkeit anpassen!
int geschwindigkeit = 15;   // Pause in ms zwischen Schritten (kleiner = schneller)
int schrittweite = 1;       // Grad pro Schritt (kleiner = sanfter)

int aktuellerWinkel = 0;    // Aktuelle Position
int richtung = 1;           // 1 = vorwaerts, -1 = rueckwaerts

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO_PIN);

    Serial.println("Challenge 2: Smooth Sweep");
    Serial.println("Sanftes Hin- und Herschwenken");
    Serial.print("Geschwindigkeit: ");
    Serial.print(geschwindigkeit);
    Serial.println(" ms/Schritt");
    Serial.println("----------------------------");

    // Startposition
    servo.write(0);
    delay(500);
}

void loop() {
    // Naechsten Schritt berechnen
    aktuellerWinkel += (schrittweite * richtung);

    // Richtung umkehren an den Grenzen
    if (aktuellerWinkel >= 180) {
        aktuellerWinkel = 180;
        richtung = -1;
        Serial.println(">> Richtungswechsel: zurueck");
    }
    else if (aktuellerWinkel <= 0) {
        aktuellerWinkel = 0;
        richtung = 1;
        Serial.println(">> Richtungswechsel: vorwaerts");
    }

    // Servo auf neuen Winkel setzen
    servo.write(aktuellerWinkel);

    // Winkel ausgeben (nur alle 10 Grad fuer Uebersichtlichkeit)
    if (aktuellerWinkel % 10 == 0) {
        Serial.print("Winkel: ");
        Serial.print(aktuellerWinkel);
        Serial.println(" Grad");
    }

    // Pause fuer sanfte Bewegung
    delay(geschwindigkeit);
}
