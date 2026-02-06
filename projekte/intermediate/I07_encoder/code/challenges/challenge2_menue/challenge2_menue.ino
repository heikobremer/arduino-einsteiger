// I07 Challenge 2: LCD-Menue mit Encoder
// Encoder dreht = Menue durchblaettern
// Taster = aktuelle Option auswaehlen

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Encoder-Pins
#define CLK_PIN 2   // Clock - Interrupt-faehig
#define DT_PIN 3    // Data
#define SW_PIN 4    // Switch (Taster)

// LCD mit I2C-Adresse 0x27 (16 Zeichen, 2 Zeilen)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Menue-Optionen
const char* menuItems[] = {
    "LED An/Aus",
    "Helligkeit",
    "Ton spielen",
    "Reset",
    "Info anzeigen"
};
const int menuCount = 5;

// Aktuelle Position im Menue
volatile int menuPosition = 0;
volatile bool encoderMoved = false;

// Entprellung fuer Taster
#define DEBOUNCE_TIME 50
unsigned long lastButtonTime = 0;
bool buttonPressed = false;

// Pfeil-Zeichen fuer Anzeige
byte arrowChar[] = {
    0b00000,
    0b00100,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b00100,
    0b00000
};

void setup() {
    Serial.begin(9600);

    // Encoder-Pins als Input mit Pull-Up
    pinMode(CLK_PIN, INPUT_PULLUP);
    pinMode(DT_PIN, INPUT_PULLUP);
    pinMode(SW_PIN, INPUT_PULLUP);

    // LCD initialisieren
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, arrowChar);  // Pfeil als Custom Char

    // Interrupt auf CLK-Pin
    attachInterrupt(digitalPinToInterrupt(CLK_PIN), encoderISR, CHANGE);

    Serial.println("Challenge 2: LCD-Menue");
    Serial.println("Drehen = navigieren");
    Serial.println("Druecken = auswaehlen");
    Serial.println("----------------------");

    // Menue anzeigen
    updateDisplay();
}

// Interrupt Service Routine
void encoderISR() {
    if (digitalRead(CLK_PIN) != digitalRead(DT_PIN)) {
        // Rechtsdrehung = naechster Eintrag
        menuPosition++;
        if (menuPosition >= menuCount) menuPosition = 0;  // Wrap around
    } else {
        // Linksdrehung = vorheriger Eintrag
        menuPosition--;
        if (menuPosition < 0) menuPosition = menuCount - 1;  // Wrap around
    }
    encoderMoved = true;
}

void updateDisplay() {
    lcd.clear();

    // Zeile 1: Ueberschrift
    lcd.setCursor(0, 0);
    lcd.print("MENUE:");

    // Zeile 2: Aktueller Eintrag mit Pfeil
    lcd.setCursor(0, 1);
    lcd.write(0);  // Pfeil-Zeichen
    lcd.print(" ");
    lcd.print(menuItems[menuPosition]);

    // Serial-Ausgabe
    Serial.print("Position: ");
    Serial.print(menuPosition + 1);
    Serial.print("/");
    Serial.print(menuCount);
    Serial.print(" -> ");
    Serial.println(menuItems[menuPosition]);
}

void selectMenuItem() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gewaehlt:");
    lcd.setCursor(0, 1);
    lcd.print(menuItems[menuPosition]);

    Serial.print("*** AUSGEWAEHLT: ");
    Serial.print(menuItems[menuPosition]);
    Serial.println(" ***");

    // Hier koenntest du die jeweilige Aktion ausfuehren
    // z.B. switch(menuPosition) { case 0: ... }

    delay(1500);  // Kurz anzeigen
    updateDisplay();
}

void checkButton() {
    unsigned long currentTime = millis();

    if (digitalRead(SW_PIN) == LOW) {
        if (!buttonPressed) {
            if (currentTime - lastButtonTime > DEBOUNCE_TIME) {
                buttonPressed = true;
                lastButtonTime = currentTime;

                // Menue-Eintrag auswaehlen
                selectMenuItem();
            }
        }
    } else {
        buttonPressed = false;
    }
}

void loop() {
    // Display aktualisieren wenn Encoder gedreht wurde
    if (encoderMoved) {
        encoderMoved = false;
        updateDisplay();
    }

    // Taster pruefen
    checkButton();

    delay(1);
}
