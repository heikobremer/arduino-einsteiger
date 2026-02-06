// I03 Challenge 3: Taster-Steuerung
// 2 Taster: Links/Rechts drehen in 10-Grad-Schritten

#include <Servo.h>

#define SERVO_PIN 9
#define TASTER_LINKS 2    // Taster fuer Linksdrehung
#define TASTER_RECHTS 3   // Taster fuer Rechtsdrehung

Servo servo;

int aktuellerWinkel = 90;  // Startposition: Mitte
int schrittweite = 10;     // Grad pro Tastendruck

// Entprellen
unsigned long letzterDruck = 0;
const unsigned long entprellZeit = 200;  // 200ms Entprellzeit

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO_PIN);

    // Taster mit internem Pull-Up
    pinMode(TASTER_LINKS, INPUT_PULLUP);
    pinMode(TASTER_RECHTS, INPUT_PULLUP);

    Serial.println("Challenge 3: Taster-Steuerung");
    Serial.println("Linker Taster = -10 Grad");
    Serial.println("Rechter Taster = +10 Grad");
    Serial.println("----------------------------");

    // Startposition anfahren
    servo.write(aktuellerWinkel);
    Serial.print("Startwinkel: ");
    Serial.println(aktuellerWinkel);
}

void loop() {
    unsigned long jetztZeit = millis();

    // Linker Taster gedrueckt? (LOW wegen Pull-Up)
    if (digitalRead(TASTER_LINKS) == LOW) {
        if (jetztZeit - letzterDruck > entprellZeit) {
            // Nach links drehen (Winkel verringern)
            aktuellerWinkel -= schrittweite;

            // Minimum pruefen
            if (aktuellerWinkel < 0) {
                aktuellerWinkel = 0;
                Serial.println("! Minimum erreicht (0 Grad)");
            }

            servo.write(aktuellerWinkel);
            Serial.print("<< Links: ");
            Serial.print(aktuellerWinkel);
            Serial.println(" Grad");

            letzterDruck = jetztZeit;
        }
    }

    // Rechter Taster gedrueckt?
    if (digitalRead(TASTER_RECHTS) == LOW) {
        if (jetztZeit - letzterDruck > entprellZeit) {
            // Nach rechts drehen (Winkel erhoehen)
            aktuellerWinkel += schrittweite;

            // Maximum pruefen
            if (aktuellerWinkel > 180) {
                aktuellerWinkel = 180;
                Serial.println("! Maximum erreicht (180 Grad)");
            }

            servo.write(aktuellerWinkel);
            Serial.print(">> Rechts: ");
            Serial.print(aktuellerWinkel);
            Serial.println(" Grad");

            letzterDruck = jetztZeit;
        }
    }

    // Kurze Pause
    delay(10);
}
