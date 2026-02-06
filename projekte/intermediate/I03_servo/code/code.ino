// I03: SG90 Servo-Motor mit PWM
// Praezise Winkelsteuerung von 0 bis 180 Grad

#include <Servo.h>

// Servo an Pin 9 (PWM-faehig)
#define SERVO_PIN 9

Servo servo;

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO_PIN);

    Serial.println("SG90 Servo-Motor Steuerung");
    Serial.println("------------------------------");
}

void loop() {
    // Von 0 bis 180 Grad
    Serial.println("Schwenke von 0 bis 180 Grad...");
    for (int angle = 0; angle <= 180; angle += 10) {
        servo.write(angle);
        Serial.print("  Winkel: ");
        Serial.print(angle);
        Serial.println(" Grad");
        delay(300);
    }

    delay(1000);

    // Von 180 zurueck zu 0 Grad
    Serial.println("Schwenke von 180 bis 0 Grad...");
    for (int angle = 180; angle >= 0; angle -= 10) {
        servo.write(angle);
        Serial.print("  Winkel: ");
        Serial.print(angle);
        Serial.println(" Grad");
        delay(300);
    }

    delay(1000);
}
