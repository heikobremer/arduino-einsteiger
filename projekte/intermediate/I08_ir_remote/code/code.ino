// I08: VS1838B IR-Empfaenger
// Infrarot-Fernbedienung mit NEC-Protokoll

#include <IRremote.h>

// IR-Empfaenger an Pin 7
#define IR_PIN 7

void setup() {
    Serial.begin(9600);

    // IR-Empfaenger initialisieren
    IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

    Serial.println("VS1838B IR-Empfaenger");
    Serial.println("------------------------------");
    Serial.println("Warte auf IR-Signale...");
    Serial.println();
}

// Bekannte Tasten-Codes (Wokwi IR-Fernbedienung, NEC-Protokoll)
void printKeyName(uint8_t code) {
    switch (code) {
        case 0xA2: Serial.print("Power"); break;
        case 0xE2: Serial.print("Menu"); break;
        case 0x22: Serial.print("Test"); break;
        case 0x02: Serial.print("Plus"); break;
        case 0xC2: Serial.print("Back"); break;
        case 0xE0: Serial.print("Prev"); break;
        case 0xA8: Serial.print("Play"); break;
        case 0x90: Serial.print("Next"); break;
        case 0x68: Serial.print("0"); break;
        case 0x98: Serial.print("Minus"); break;
        case 0xB0: Serial.print("C"); break;
        case 0x30: Serial.print("1"); break;
        case 0x18: Serial.print("2"); break;
        case 0x7A: Serial.print("3"); break;
        case 0x10: Serial.print("4"); break;
        case 0x38: Serial.print("5"); break;
        case 0x5A: Serial.print("6"); break;
        case 0x42: Serial.print("7"); break;
        case 0x4A: Serial.print("8"); break;
        case 0x52: Serial.print("9"); break;
        default: Serial.print("Unbekannt"); break;
    }
}

void loop() {
    // Pruefen ob IR-Signal empfangen wurde
    if (IrReceiver.decode()) {
        // Signal-Informationen ausgeben
        if (IrReceiver.decodedIRData.protocol != UNKNOWN) {
            Serial.print("Taste: ");
            printKeyName(IrReceiver.decodedIRData.command);

            Serial.print(" | Code: 0x");
            Serial.print(IrReceiver.decodedIRData.command, HEX);

            Serial.print(" | Protokoll: ");
            Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));

            // Repeat-Signal erkennen
            if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
                Serial.println("  (Repeat)");
            }
        } else {
            // Unbekanntes Protokoll - rohe Daten ausgeben
            Serial.print("Unbekanntes Signal: 0x");
            Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        }

        // Empfaenger fuer naechstes Signal vorbereiten
        IrReceiver.resume();
    }
}
