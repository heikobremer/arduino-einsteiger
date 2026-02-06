// I07 Challenge 3: RGB-LED mit Encoder steuern
// Encoder dreht = Farbwert aendern (0-255)
// Taster = zwischen R/G/B Kanal wechseln

// Encoder-Pins
#define CLK_PIN 2   // Clock - Interrupt-faehig
#define DT_PIN 3    // Data
#define SW_PIN 4    // Switch (Taster)

// RGB-LED Pins (PWM-faehig!)
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

// Farbwerte (0-255)
int farbwerte[3] = {255, 0, 0};  // Start: Rot
const char* kanalNamen[] = {"ROT", "GRUEN", "BLAU"};

// Aktueller Kanal (0=R, 1=G, 2=B)
int aktuellerKanal = 0;

// Encoder-Status
volatile int encoderDelta = 0;
volatile bool encoderMoved = false;

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

    // RGB-LED Pins als Output
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    // Interrupt auf CLK-Pin
    attachInterrupt(digitalPinToInterrupt(CLK_PIN), encoderISR, CHANGE);

    Serial.println("Challenge 3: RGB-Farbe");
    Serial.println("Drehen = Farbwert aendern");
    Serial.println("Druecken = Kanal wechseln (R->G->B)");
    Serial.println("------------------------------------");

    // Farbe setzen
    updateLED();
    printStatus();
}

// Interrupt Service Routine
void encoderISR() {
    if (digitalRead(CLK_PIN) != digitalRead(DT_PIN)) {
        encoderDelta = 5;   // Rechtsdrehung
    } else {
        encoderDelta = -5;  // Linksdrehung
    }
    encoderMoved = true;
}

void updateLED() {
    analogWrite(RED_PIN, farbwerte[0]);
    analogWrite(GREEN_PIN, farbwerte[1]);
    analogWrite(BLUE_PIN, farbwerte[2]);
}

void printStatus() {
    Serial.print("Kanal: ");
    Serial.print(kanalNamen[aktuellerKanal]);
    Serial.print(" | RGB(");
    Serial.print(farbwerte[0]);
    Serial.print(", ");
    Serial.print(farbwerte[1]);
    Serial.print(", ");
    Serial.print(farbwerte[2]);
    Serial.println(")");
}

void checkButton() {
    unsigned long currentTime = millis();

    if (digitalRead(SW_PIN) == LOW) {
        if (!buttonPressed) {
            if (currentTime - lastButtonTime > DEBOUNCE_TIME) {
                buttonPressed = true;
                lastButtonTime = currentTime;

                // Naechsten Kanal waehlen
                aktuellerKanal = (aktuellerKanal + 1) % 3;

                Serial.print("*** Kanal gewechselt: ");
                Serial.print(kanalNamen[aktuellerKanal]);
                Serial.println(" ***");

                printStatus();
            }
        }
    } else {
        buttonPressed = false;
    }
}

void loop() {
    // Farbwert aendern wenn Encoder gedreht wurde
    if (encoderMoved) {
        encoderMoved = false;

        // Aktuellen Kanal anpassen
        farbwerte[aktuellerKanal] += encoderDelta;

        // Wert begrenzen (0-255)
        if (farbwerte[aktuellerKanal] > 255) farbwerte[aktuellerKanal] = 255;
        if (farbwerte[aktuellerKanal] < 0) farbwerte[aktuellerKanal] = 0;

        // LED aktualisieren
        updateLED();
        printStatus();
    }

    // Taster pruefen
    checkButton();

    delay(1);
}
