// I08 Challenge 4: Servo mit IR-Fernbedienung steuern
// PREV/NEXT Tasten bewegen den Servo links/rechts

#include <IRremote.h>
#include <Servo.h>

// IR-Empfaenger an Pin 7
#define IR_PIN 7

// Servo an PWM-faehigem Pin 9
#define SERVO_PIN 9

// Servo-Objekt erstellen
Servo servo;

// Aktuelle Servo-Position (0-180 Grad)
int position = 90;  // Mittelstellung

// Schrittweite fuer Bewegung
#define SCHRITT 15

// IR-Tasten-Codes (Wokwi Fernbedienung, NEC-Protokoll)
#define KEY_PREV  0xE0   // Links drehen
#define KEY_NEXT  0x90   // Rechts drehen
#define KEY_PLAY  0xA8   // Mitte (90 Grad)
#define KEY_0     0x68   // Minimum (0 Grad)
#define KEY_9     0x52   // Maximum (180 Grad)

void setup() {
    Serial.begin(9600);

    // Servo initialisieren
    servo.attach(SERVO_PIN);

    // IR-Empfaenger initialisieren
    IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

    // Servo in Mittelstellung bringen
    servo.write(position);

    Serial.println("I08 Challenge 4: Servo-Steuerung");
    Serial.println("--------------------------------");
    Serial.println("PREV = Links drehen");
    Serial.println("NEXT = Rechts drehen");
    Serial.println("PLAY = Mitte (90 Grad)");
    Serial.println("0    = Minimum (0 Grad)");
    Serial.println("9    = Maximum (180 Grad)");
    Serial.println();
    Serial.print("Aktuelle Position: ");
    Serial.print(position);
    Serial.println(" Grad");
}

void bewegeServo(int neuePosition) {
    // Grenzen pruefen
    if (neuePosition < 0) neuePosition = 0;
    if (neuePosition > 180) neuePosition = 180;

    // Position speichern und setzen
    position = neuePosition;
    servo.write(position);

    // Im Serial Monitor anzeigen
    Serial.print("Servo-Position: ");
    Serial.print(position);
    Serial.println(" Grad");
}

void loop() {
    // Pruefen ob IR-Signal empfangen wurde
    if (IrReceiver.decode()) {

        // Nur bekannte Protokolle verarbeiten
        if (IrReceiver.decodedIRData.protocol != UNKNOWN) {

            uint8_t command = IrReceiver.decodedIRData.command;

            switch (command) {
                case KEY_PREV:
                    // Links drehen (Winkel verringern)
                    Serial.print("Links: ");
                    bewegeServo(position - SCHRITT);
                    break;

                case KEY_NEXT:
                    // Rechts drehen (Winkel erhoehen)
                    Serial.print("Rechts: ");
                    bewegeServo(position + SCHRITT);
                    break;

                case KEY_PLAY:
                    // Mittelstellung
                    Serial.print("Mitte: ");
                    bewegeServo(90);
                    break;

                case KEY_0:
                    // Minimum-Position
                    Serial.print("Minimum: ");
                    bewegeServo(0);
                    break;

                case KEY_9:
                    // Maximum-Position
                    Serial.print("Maximum: ");
                    bewegeServo(180);
                    break;

                default:
                    // Andere Tasten ignorieren
                    break;
            }
        }

        // Empfaenger fuer naechstes Signal vorbereiten
        IrReceiver.resume();
    }
}
