// P01: LCD Keypad Demo
// LCD I2C mit 5 digitalen Buttons
// Gut strukturiert mit Konstanten, Enum und Hilfsfunktionen

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===== PIN-DEFINITIONEN =====
const int PIN_BTN_RIGHT  = 2;
const int PIN_BTN_UP     = 3;
const int PIN_BTN_DOWN   = 4;
const int PIN_BTN_LEFT   = 5;
const int PIN_BTN_SELECT = 6;

// ===== TASTEN-ENUM =====
enum Button {
    BTN_NONE,
    BTN_RIGHT,
    BTN_UP,
    BTN_DOWN,
    BTN_LEFT,
    BTN_SELECT
};

// ===== LCD-OBJEKT (I2C Adresse 0x27, 16 Zeichen, 2 Zeilen) =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== HILFSFUNKTIONEN =====

// Liest die digitalen Buttons und gibt die gedrueckte Taste zurueck
Button readButton() {
    // Buttons sind LOW-aktiv (INPUT_PULLUP)
    if (digitalRead(PIN_BTN_RIGHT) == LOW)  return BTN_RIGHT;
    if (digitalRead(PIN_BTN_UP) == LOW)     return BTN_UP;
    if (digitalRead(PIN_BTN_DOWN) == LOW)   return BTN_DOWN;
    if (digitalRead(PIN_BTN_LEFT) == LOW)   return BTN_LEFT;
    if (digitalRead(PIN_BTN_SELECT) == LOW) return BTN_SELECT;
    return BTN_NONE;
}

// Gibt den deutschen Namen der Taste zurueck
const char* getButtonName(Button button) {
    switch (button) {
        case BTN_RIGHT:  return "Rechts";
        case BTN_UP:     return "Hoch";
        case BTN_DOWN:   return "Runter";
        case BTN_LEFT:   return "Links";
        case BTN_SELECT: return "Select";
        default:         return "Keine";
    }
}

// Zeigt den Startbildschirm
void showWelcomeScreen() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LCD Keypad Demo");
    lcd.setCursor(0, 1);
    lcd.print("Taste druecken!");
}

// Zeigt die aktuelle Taste
void showButton(Button button) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Taste:");
    lcd.setCursor(0, 1);
    lcd.print(getButtonName(button));
}

// ===== SETUP =====
void setup() {
    // Button-Pins als Input mit internem Pull-up
    pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_BTN_UP, INPUT_PULLUP);
    pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
    pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
    pinMode(PIN_BTN_SELECT, INPUT_PULLUP);

    // LCD initialisieren
    lcd.init();
    lcd.backlight();

    // Serial fuer Debugging
    Serial.begin(9600);
    Serial.println("LCD Keypad Demo gestartet");

    // Startbildschirm anzeigen
    showWelcomeScreen();
}

// ===== HAUPTSCHLEIFE =====
void loop() {
    static Button lastButton = BTN_NONE;
    static unsigned long lastPressTime = 0;

    Button currentButton = readButton();

    // Nur bei Tastenwechsel reagieren
    if (currentButton != lastButton) {
        lastButton = currentButton;
        lastPressTime = millis();

        if (currentButton != BTN_NONE) {
            showButton(currentButton);

            // Debug-Ausgabe
            Serial.print("Taste: ");
            Serial.println(getButtonName(currentButton));
        }
    }

    // Nach 3 Sekunden ohne Eingabe: Willkommensbildschirm
    if (currentButton == BTN_NONE && millis() - lastPressTime > 3000) {
        showWelcomeScreen();
        lastPressTime = millis();
    }

    delay(50);  // Entprellen
}
