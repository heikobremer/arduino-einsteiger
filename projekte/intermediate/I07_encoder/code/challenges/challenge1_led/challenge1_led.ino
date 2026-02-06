// I07 Challenge 1: LED-Helligkeit mit Encoder steuern
// Drehe am Encoder = LED wird heller/dunkler (PWM 0-255)
// Taster = LED aus (Helligkeit auf 0)

// Encoder-Pins
#define CLK_PIN 2   // Clock - Interrupt-faehig
#define DT_PIN 3    // Data
#define SW_PIN 4    // Switch (Taster)

// LED an PWM-faehigem Pin
#define LED_PIN 9   // PWM-faehig!

// Helligkeit (0-255)
volatile int helligkeit = 128;  // Startwert: halbe Helligkeit
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

    // LED als Output
    pinMode(LED_PIN, OUTPUT);

    // Interrupt auf CLK-Pin
    attachInterrupt(digitalPinToInterrupt(CLK_PIN), encoderISR, CHANGE);

    Serial.println("Challenge 1: LED-Helligkeit");
    Serial.println("Drehen = Helligkeit aendern");
    Serial.println("Druecken = LED aus");
    Serial.println("---------------------------");

    // LED mit Startwert setzen
    analogWrite(LED_PIN, helligkeit);
}

// Interrupt Service Routine
void encoderISR() {
    if (digitalRead(CLK_PIN) != digitalRead(DT_PIN)) {
        // Rechtsdrehung = heller
        helligkeit += 5;
        if (helligkeit > 255) helligkeit = 255;
    } else {
        // Linksdrehung = dunkler
        helligkeit -= 5;
        if (helligkeit < 0) helligkeit = 0;
    }
    encoderMoved = true;
}

void checkButton() {
    unsigned long currentTime = millis();

    if (digitalRead(SW_PIN) == LOW) {
        if (!buttonPressed) {
            if (currentTime - lastButtonTime > DEBOUNCE_TIME) {
                buttonPressed = true;
                lastButtonTime = currentTime;

                // LED ausschalten
                helligkeit = 0;
                analogWrite(LED_PIN, helligkeit);
                Serial.println("*** LED AUS ***");
            }
        }
    } else {
        buttonPressed = false;
    }
}

void loop() {
    // Helligkeit aktualisieren wenn Encoder gedreht wurde
    if (encoderMoved) {
        encoderMoved = false;

        // LED-Helligkeit setzen (PWM)
        analogWrite(LED_PIN, helligkeit);

        // Anzeige im Serial Monitor
        Serial.print("Helligkeit: ");
        Serial.print(helligkeit);
        Serial.print(" / 255 (");
        Serial.print((helligkeit * 100) / 255);
        Serial.println("%)");
    }

    // Taster pruefen
    checkButton();

    delay(1);
}
