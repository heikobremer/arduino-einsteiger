// I08 Challenge 2: LED-Dimmer mit IR-Fernbedienung
// VOL+ erhoeht die Helligkeit, VOL- verringert sie

#include <IRremote.h>

// IR-Empfaenger an Pin 7
#define IR_PIN 7

// LED an PWM-faehigem Pin 9
#define LED_PIN 9

// Aktuelle Helligkeit (0-255)
int helligkeit = 128;

// Schrittweite fuer Helligkeitsaenderung
#define SCHRITT 25

// IR-Tasten-Codes (Wokwi Fernbedienung, NEC-Protokoll)
#define KEY_VOL_MINUS 0x98   // Minus-Taste
#define KEY_VOL_PLUS  0x02   // Plus-Taste
#define KEY_0         0x68   // Aus (Helligkeit = 0)
#define KEY_PLAY      0xA8   // Maximum (Helligkeit = 255)

void setup() {
    Serial.begin(9600);

    // LED-Pin als Ausgang konfigurieren
    pinMode(LED_PIN, OUTPUT);

    // IR-Empfaenger initialisieren
    IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

    // Initiale Helligkeit setzen
    analogWrite(LED_PIN, helligkeit);

    Serial.println("I08 Challenge 2: LED-Dimmer");
    Serial.println("---------------------------");
    Serial.println("VOL+ = Heller");
    Serial.println("VOL- = Dunkler");
    Serial.println("PLAY = Maximum (255)");
    Serial.println("0    = Aus (0)");
    Serial.println();
    Serial.print("Aktuelle Helligkeit: ");
    Serial.println(helligkeit);
}

void loop() {
    // Pruefen ob IR-Signal empfangen wurde
    if (IrReceiver.decode()) {

        // Nur bekannte Protokolle verarbeiten
        if (IrReceiver.decodedIRData.protocol != UNKNOWN) {

            uint8_t command = IrReceiver.decodedIRData.command;

            switch (command) {
                case KEY_VOL_PLUS:
                    // Helligkeit erhoehen
                    helligkeit += SCHRITT;
                    if (helligkeit > 255) helligkeit = 255;
                    Serial.print("Heller: ");
                    Serial.println(helligkeit);
                    break;

                case KEY_VOL_MINUS:
                    // Helligkeit verringern
                    helligkeit -= SCHRITT;
                    if (helligkeit < 0) helligkeit = 0;
                    Serial.print("Dunkler: ");
                    Serial.println(helligkeit);
                    break;

                case KEY_PLAY:
                    // Maximale Helligkeit
                    helligkeit = 255;
                    Serial.println("Maximum: 255");
                    break;

                case KEY_0:
                    // LED ausschalten
                    helligkeit = 0;
                    Serial.println("Aus: 0");
                    break;

                default:
                    // Andere Tasten ignorieren
                    break;
            }

            // Neue Helligkeit an LED ausgeben
            analogWrite(LED_PIN, helligkeit);

            // Prozent-Anzeige
            int prozent = map(helligkeit, 0, 255, 0, 100);
            Serial.print("Helligkeit: ");
            Serial.print(prozent);
            Serial.println("%");
        }

        // Empfaenger fuer naechstes Signal vorbereiten
        IrReceiver.resume();
    }
}
