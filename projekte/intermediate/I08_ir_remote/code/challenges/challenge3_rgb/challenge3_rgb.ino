// I08 Challenge 3: RGB-LED mit IR-Fernbedienung steuern
// Zifferntasten 1-6 waehlen verschiedene Farben

#include <IRremote.h>

// IR-Empfaenger an Pin 7
#define IR_PIN 7

// RGB-LED an PWM-faehigen Pins (Common Cathode)
#define RED_PIN   9
#define GREEN_PIN 10
#define BLUE_PIN  11

// IR-Tasten-Codes (Wokwi Fernbedienung, NEC-Protokoll)
#define KEY_0 0x68   // Aus
#define KEY_1 0x30   // Rot
#define KEY_2 0x18   // Gruen
#define KEY_3 0x7A   // Blau
#define KEY_4 0x10   // Gelb
#define KEY_5 0x38   // Cyan
#define KEY_6 0x5A   // Magenta
#define KEY_7 0x42   // Weiss

// Aktuelle Farbe speichern
int aktuelleRot = 0;
int aktuelleGruen = 0;
int aktuelleBlau = 0;

void setzeRGB(int r, int g, int b) {
    // Farbwerte setzen
    aktuelleRot = r;
    aktuelleGruen = g;
    aktuelleBlau = b;

    // An LED ausgeben
    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);

    // Im Serial Monitor anzeigen
    Serial.print("RGB: (");
    Serial.print(r);
    Serial.print(", ");
    Serial.print(g);
    Serial.print(", ");
    Serial.print(b);
    Serial.println(")");
}

void setup() {
    Serial.begin(9600);

    // RGB-Pins als Ausgang konfigurieren
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    // IR-Empfaenger initialisieren
    IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

    Serial.println("I08 Challenge 3: RGB-LED Farbwechsel");
    Serial.println("------------------------------------");
    Serial.println("0 = Aus");
    Serial.println("1 = Rot");
    Serial.println("2 = Gruen");
    Serial.println("3 = Blau");
    Serial.println("4 = Gelb");
    Serial.println("5 = Cyan");
    Serial.println("6 = Magenta");
    Serial.println("7 = Weiss");
    Serial.println();

    // Initial ausschalten
    setzeRGB(0, 0, 0);
}

void loop() {
    // Pruefen ob IR-Signal empfangen wurde
    if (IrReceiver.decode()) {

        // Nur bekannte Protokolle verarbeiten
        if (IrReceiver.decodedIRData.protocol != UNKNOWN) {

            // Repeat-Signale ignorieren
            if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {

                uint8_t command = IrReceiver.decodedIRData.command;

                switch (command) {
                    case KEY_0:
                        Serial.print("Farbe: Aus - ");
                        setzeRGB(0, 0, 0);
                        break;

                    case KEY_1:
                        Serial.print("Farbe: Rot - ");
                        setzeRGB(255, 0, 0);
                        break;

                    case KEY_2:
                        Serial.print("Farbe: Gruen - ");
                        setzeRGB(0, 255, 0);
                        break;

                    case KEY_3:
                        Serial.print("Farbe: Blau - ");
                        setzeRGB(0, 0, 255);
                        break;

                    case KEY_4:
                        Serial.print("Farbe: Gelb - ");
                        setzeRGB(255, 255, 0);
                        break;

                    case KEY_5:
                        Serial.print("Farbe: Cyan - ");
                        setzeRGB(0, 255, 255);
                        break;

                    case KEY_6:
                        Serial.print("Farbe: Magenta - ");
                        setzeRGB(255, 0, 255);
                        break;

                    case KEY_7:
                        Serial.print("Farbe: Weiss - ");
                        setzeRGB(255, 255, 255);
                        break;

                    default:
                        // Andere Tasten ignorieren
                        break;
                }
            }
        }

        // Empfaenger fuer naechstes Signal vorbereiten
        IrReceiver.resume();
    }
}
