// I07: Rotary Encoder (KY-040)
// Drehgeber mit Taster fuer Menusteuerung

// Encoder-Pins (CLK auf Pin 2 fuer Interrupt)
#define CLK_PIN 2   // Clock (A) - Interrupt-faehig
#define DT_PIN 3    // Data (B)
#define SW_PIN 4    // Switch (Taster)

// Zaehler und Status
volatile int counter = 0;
volatile bool encoderMoved = false;
int lastCounter = 0;

// Entprellzeit in Millisekunden
#define DEBOUNCE_TIME 50
unsigned long lastButtonTime = 0;
bool buttonPressed = false;

void setup() {
    Serial.begin(9600);

    // Pins konfigurieren mit Pull-Up
    pinMode(CLK_PIN, INPUT_PULLUP);
    pinMode(DT_PIN, INPUT_PULLUP);
    pinMode(SW_PIN, INPUT_PULLUP);

    // Interrupt auf CLK-Pin (Pin 2 = Interrupt 0)
    attachInterrupt(digitalPinToInterrupt(CLK_PIN), encoderISR, CHANGE);

    Serial.println("Rotary Encoder (KY-040)");
    Serial.println("------------------------------");
    Serial.println("Drehen: Zaehler aendern");
    Serial.println("Druecken: Zaehler zuruecksetzen");
    Serial.println();
}

// Interrupt Service Routine fuer Encoder
void encoderISR() {
    // Richtung bestimmen durch Vergleich CLK mit DT
    if (digitalRead(CLK_PIN) != digitalRead(DT_PIN)) {
        counter++;
    } else {
        counter--;
    }
    encoderMoved = true;
}

void checkButton() {
    unsigned long currentTime = millis();

    if (digitalRead(SW_PIN) == LOW) {  // Taster gedrueckt (LOW aktiv)
        if (!buttonPressed) {
            if (currentTime - lastButtonTime > DEBOUNCE_TIME) {
                buttonPressed = true;
                lastButtonTime = currentTime;

                // Aktion bei Tastendruck
                counter = 0;
                Serial.println("*** RESET - Zaehler: 0 ***");
            }
        }
    } else {
        buttonPressed = false;
    }
}

void loop() {
    // Encoder-Aenderung anzeigen
    if (encoderMoved) {
        encoderMoved = false;

        // Richtung bestimmen
        String direction = (counter > lastCounter) ? "rechts" : "links ";

        Serial.print("Drehung: ");
        Serial.print(direction);
        Serial.print(" | Zaehler: ");
        Serial.println(counter);

        lastCounter = counter;
    }

    // Taster pruefen
    checkButton();

    delay(1);  // Kurze Pause fuer stabiles Lesen
}
