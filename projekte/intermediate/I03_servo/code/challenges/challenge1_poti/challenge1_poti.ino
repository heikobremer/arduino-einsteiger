// I03 Challenge 1: Potentiometer steuert Servo-Winkel
// Drehe am Poti = Servo folgt proportional!

#include <Servo.h>

#define SERVO_PIN 9     // Servo an PWM-Pin 9
#define POTI_PIN A0     // Potentiometer an Analog-Pin A0

Servo servo;
int potiWert = 0;       // Rohwert vom Poti (0-1023)
int winkel = 0;         // Umgerechneter Winkel (0-180)

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO_PIN);

    Serial.println("Challenge 1: Poti-Steuerung");
    Serial.println("Drehe am Potentiometer!");
    Serial.println("----------------------------");
}

void loop() {
    // Potentiometer auslesen (0-1023)
    potiWert = analogRead(POTI_PIN);

    // Wert auf Servo-Bereich umrechnen (0-180 Grad)
    winkel = map(potiWert, 0, 1023, 0, 180);

    // Servo auf berechneten Winkel setzen
    servo.write(winkel);

    // Ausgabe im Serial Monitor
    Serial.print("Poti: ");
    Serial.print(potiWert);
    Serial.print(" -> Winkel: ");
    Serial.print(winkel);
    Serial.println(" Grad");

    // Kleine Pause fuer stabiles Signal
    delay(15);
}
