// I07 Challenge 4: Servo-Winkel mit Encoder steuern
// Encoder dreht = Servo-Position aendern (0-180 Grad)
// Taster = Servo auf Mittelposition (90 Grad)

#include <Servo.h>

// Encoder-Pins
#define CLK_PIN 2   // Clock - Interrupt-faehig
#define DT_PIN 3    // Data
#define SW_PIN 4    // Switch (Taster)

// Servo an PWM-Pin
#define SERVO_PIN 9

// Servo-Objekt
Servo servo;

// Winkel (0-180 Grad)
volatile int winkel = 90;  // Start: Mittelposition
volatile bool encoderMoved = false;

// Schrittweite pro Encoder-Rastung
#define SCHRITT 3

// Entprellung fuer Taster
#define DEBOUNCE_TIME 50
unsigned long lastButtonTime = 0;
bool buttonPressed = false;

void setup() {
    Serial.begin(9600);

    // Encoder-Pins als Input mit Pull-Up
    pinMode(CLK_PIN, INPUT_PULLUP);
    pinMode(DT_PIN, INPUT_PULLUP);
    pinMode(SW_PIN, INPUT_PULLUP);

    // Servo initialisieren
    servo.attach(SERVO_PIN);
    servo.write(winkel);

    // Interrupt auf CLK-Pin
    attachInterrupt(digitalPinToInterrupt(CLK_PIN), encoderISR, CHANGE);

    Serial.println("Challenge 4: Servo-Steuerung");
    Serial.println("Drehen = Winkel aendern");
    Serial.println("Druecken = Mittelposition (90 Grad)");
    Serial.println("-----------------------------------");

    printPosition();
}

// Interrupt Service Routine
void encoderISR() {
    if (digitalRead(CLK_PIN) != digitalRead(DT_PIN)) {
        // Rechtsdrehung = groesserer Winkel
        winkel += SCHRITT;
        if (winkel > 180) winkel = 180;
    } else {
        // Linksdrehung = kleinerer Winkel
        winkel -= SCHRITT;
        if (winkel < 0) winkel = 0;
    }
    encoderMoved = true;
}

void printPosition() {
    Serial.print("Servo-Position: ");
    Serial.print(winkel);
    Serial.print(" Grad");

    // Visuelle Anzeige
    Serial.print(" [");
    int balken = map(winkel, 0, 180, 0, 20);
    for (int i = 0; i < 20; i++) {
        if (i == balken) {
            Serial.print("|");
        } else {
            Serial.print("-");
        }
    }
    Serial.println("]");
}

void checkButton() {
    unsigned long currentTime = millis();

    if (digitalRead(SW_PIN) == LOW) {
        if (!buttonPressed) {
            if (currentTime - lastButtonTime > DEBOUNCE_TIME) {
                buttonPressed = true;
                lastButtonTime = currentTime;

                // Auf Mittelposition zuruecksetzen
                winkel = 90;
                servo.write(winkel);

                Serial.println("*** MITTELPOSITION (90 Grad) ***");
                printPosition();
            }
        }
    } else {
        buttonPressed = false;
    }
}

void loop() {
    // Servo-Position aktualisieren wenn Encoder gedreht wurde
    if (encoderMoved) {
        encoderMoved = false;

        // Servo auf neuen Winkel setzen
        servo.write(winkel);
        printPosition();
    }

    // Taster pruefen
    checkButton();

    delay(1);
}
