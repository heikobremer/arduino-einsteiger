// I08 Challenge 1: 3 LEDs mit IR-Fernbedienung steuern
// Tasten 1, 2 und 3 schalten jeweils eine LED ein/aus (Toggle)

#include <IRremote.h>

// IR-Empfaenger an Pin 7
#define IR_PIN 7

// LEDs an Pins 8, 9 und 10
#define LED1_PIN 8
#define LED2_PIN 9
#define LED3_PIN 10

// LED-Zustaende speichern
bool led1State = false;
bool led2State = false;
bool led3State = false;

// IR-Tasten-Codes (Wokwi Fernbedienung, NEC-Protokoll)
#define KEY_1 0x30
#define KEY_2 0x18
#define KEY_3 0x7A

void setup() {
    Serial.begin(9600);

    // LED-Pins als Ausgang konfigurieren
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(LED3_PIN, OUTPUT);

    // IR-Empfaenger initialisieren
    IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

    Serial.println("I08 Challenge 1: LED-Steuerung");
    Serial.println("------------------------------");
    Serial.println("Taste 1 = Rote LED");
    Serial.println("Taste 2 = Gruene LED");
    Serial.println("Taste 3 = Blaue LED");
    Serial.println();
}

void loop() {
    // Pruefen ob IR-Signal empfangen wurde
    if (IrReceiver.decode()) {

        // Nur bekannte Protokolle verarbeiten
        if (IrReceiver.decodedIRData.protocol != UNKNOWN) {

            // Repeat-Signale ignorieren (verhindert Mehrfach-Toggle)
            if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {

                uint8_t command = IrReceiver.decodedIRData.command;

                switch (command) {
                    case KEY_1:
                        // LED 1 umschalten (Toggle)
                        led1State = !led1State;
                        digitalWrite(LED1_PIN, led1State);
                        Serial.print("LED 1 (Rot): ");
                        Serial.println(led1State ? "AN" : "AUS");
                        break;

                    case KEY_2:
                        // LED 2 umschalten (Toggle)
                        led2State = !led2State;
                        digitalWrite(LED2_PIN, led2State);
                        Serial.print("LED 2 (Gruen): ");
                        Serial.println(led2State ? "AN" : "AUS");
                        break;

                    case KEY_3:
                        // LED 3 umschalten (Toggle)
                        led3State = !led3State;
                        digitalWrite(LED3_PIN, led3State);
                        Serial.print("LED 3 (Blau): ");
                        Serial.println(led3State ? "AN" : "AUS");
                        break;

                    default:
                        Serial.print("Unbekannte Taste: 0x");
                        Serial.println(command, HEX);
                        break;
                }
            }
        }

        // Empfaenger fuer naechstes Signal vorbereiten
        IrReceiver.resume();
    }
}
