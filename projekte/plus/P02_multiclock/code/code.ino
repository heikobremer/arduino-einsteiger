// P02: Multiclock - Uhr, Stoppuhr, Alarm und Timer
// LCD I2C mit 5 digitalen Buttons und DS1307 RTC

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

// ===== PIN-DEFINITIONEN =====
const int PIN_BTN_RIGHT  = 2;
const int PIN_BTN_UP     = 3;
const int PIN_BTN_DOWN   = 4;
const int PIN_BTN_LEFT   = 5;
const int PIN_BTN_SELECT = 6;
const int PIN_LED_ALARM  = 7;   // Rote LED - Alarm aktiv
const int PIN_LED_TIMER  = 8;   // Gelbe LED - Timer laeuft
const int PIN_BUZZER     = 9;   // Piezo Buzzer

// ===== ENUMS =====
enum Button { BTN_NONE, BTN_RIGHT, BTN_UP, BTN_DOWN, BTN_LEFT, BTN_SELECT };

enum Mode {
    MODE_CLOCK,
    MODE_STOPWATCH,
    MODE_ALARM,
    MODE_TIMER
};

// ===== GLOBALE OBJEKTE =====
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;

// ===== ZUSTANDSVARIABLEN =====
Mode currentMode = MODE_CLOCK;
bool editMode = false;
int editField = 0;  // 0=Stunde, 1=Minute

// Stoppuhr
unsigned long stopwatchStart = 0;
unsigned long stopwatchElapsed = 0;
bool stopwatchRunning = false;

// Alarm
int alarmHour = 7;
int alarmMinute = 0;
bool alarmEnabled = false;
bool alarmTriggered = false;

// Timer
int timerMinutes = 5;
int timerSeconds = 0;
unsigned long timerEndTime = 0;
bool timerRunning = false;

// Entprellung
unsigned long lastButtonTime = 0;
const int DEBOUNCE_DELAY = 200;

// ===== HILFSFUNKTIONEN =====

Button readButton() {
    if (digitalRead(PIN_BTN_RIGHT) == LOW)  return BTN_RIGHT;
    if (digitalRead(PIN_BTN_UP) == LOW)     return BTN_UP;
    if (digitalRead(PIN_BTN_DOWN) == LOW)   return BTN_DOWN;
    if (digitalRead(PIN_BTN_LEFT) == LOW)   return BTN_LEFT;
    if (digitalRead(PIN_BTN_SELECT) == LOW) return BTN_SELECT;
    return BTN_NONE;
}

void printTwoDigits(int num) {
    if (num < 10) lcd.print("0");
    lcd.print(num);
}

const char* getModeName(Mode mode) {
    switch (mode) {
        case MODE_CLOCK:     return "Uhr";
        case MODE_STOPWATCH: return "Stoppuhr";
        case MODE_ALARM:     return "Alarm";
        case MODE_TIMER:     return "Timer";
        default:             return "";
    }
}

void showHeader() {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print(getModeName(currentMode));

    // Status rechts
    lcd.setCursor(13, 0);
    if (alarmEnabled) lcd.print("A");
    if (timerRunning) lcd.print("T");
}

// ===== MODUS: UHR =====

void displayClock() {
    DateTime now = rtc.now();
    showHeader();

    lcd.setCursor(0, 1);
    printTwoDigits(now.hour());
    lcd.print(":");
    printTwoDigits(now.minute());
    lcd.print(":");
    printTwoDigits(now.second());

    if (editMode) {
        lcd.setCursor(editField == 0 ? 0 : 3, 1);
        if ((millis() / 300) % 2 == 0) lcd.print("  ");
    }
}

void handleClockButtons(Button btn) {
    DateTime now = rtc.now();

    if (!editMode) {
        if (btn == BTN_SELECT) {
            editMode = true;
            editField = 0;
        }
    } else {
        int h = now.hour();
        int m = now.minute();

        if (btn == BTN_UP) {
            if (editField == 0) h = (h + 1) % 24;
            else m = (m + 1) % 60;
            rtc.adjust(DateTime(now.year(), now.month(), now.day(), h, m, 0));
        }
        if (btn == BTN_DOWN) {
            if (editField == 0) h = (h + 23) % 24;
            else m = (m + 59) % 60;
            rtc.adjust(DateTime(now.year(), now.month(), now.day(), h, m, 0));
        }
        if (btn == BTN_RIGHT) {
            editField = 1;
        }
        if (btn == BTN_LEFT) {
            editField = 0;
        }
        if (btn == BTN_SELECT && editField == 1) {
            editMode = false;
        }
    }
}

// ===== MODUS: STOPPUHR =====

void displayStopwatch() {
    if (stopwatchRunning) {
        stopwatchElapsed = millis() - stopwatchStart;
    }

    showHeader();
    lcd.setCursor(10, 0);
    lcd.print(stopwatchRunning ? "RUN" : "STP");

    int mins = stopwatchElapsed / 60000;
    int secs = (stopwatchElapsed / 1000) % 60;
    int hund = (stopwatchElapsed / 10) % 100;

    lcd.setCursor(0, 1);
    printTwoDigits(mins);
    lcd.print(":");
    printTwoDigits(secs);
    lcd.print(".");
    printTwoDigits(hund);
}

void handleStopwatchButtons(Button btn) {
    if (btn == BTN_SELECT) {
        if (stopwatchRunning) {
            stopwatchRunning = false;
        } else {
            stopwatchStart = millis() - stopwatchElapsed;
            stopwatchRunning = true;
        }
    }
    if (btn == BTN_DOWN && !stopwatchRunning) {
        stopwatchElapsed = 0;
    }
}

// ===== MODUS: ALARM =====

void displayAlarm() {
    showHeader();
    lcd.setCursor(10, 0);
    lcd.print(alarmEnabled ? "[AN]" : "[AUS]");

    lcd.setCursor(0, 1);
    lcd.print("Weck: ");
    printTwoDigits(alarmHour);
    lcd.print(":");
    printTwoDigits(alarmMinute);
}

void handleAlarmButtons(Button btn) {
    if (btn == BTN_UP) {
        alarmMinute++;
        if (alarmMinute >= 60) {
            alarmMinute = 0;
            alarmHour = (alarmHour + 1) % 24;
        }
    }
    if (btn == BTN_DOWN) {
        alarmMinute--;
        if (alarmMinute < 0) {
            alarmMinute = 59;
            alarmHour = (alarmHour + 23) % 24;
        }
    }
    if (btn == BTN_SELECT) {
        alarmEnabled = !alarmEnabled;
        alarmTriggered = false;
    }
}

void checkAlarm() {
    if (!alarmEnabled || alarmTriggered) return;

    DateTime now = rtc.now();
    if (now.hour() == alarmHour && now.minute() == alarmMinute && now.second() == 0) {
        triggerBuzzer();
        alarmTriggered = true;
    }
}

// ===== MODUS: TIMER =====

void displayTimer() {
    showHeader();
    lcd.setCursor(10, 0);
    lcd.print(timerRunning ? "[RUN]" : "[SET]");

    lcd.setCursor(0, 1);
    if (timerRunning) {
        unsigned long remaining = 0;
        if (millis() < timerEndTime) {
            remaining = timerEndTime - millis();
        }
        int m = remaining / 60000;
        int s = (remaining / 1000) % 60;
        printTwoDigits(m);
        lcd.print(":");
        printTwoDigits(s);
        lcd.print(" Rest  ");
    } else {
        printTwoDigits(timerMinutes);
        lcd.print(":");
        printTwoDigits(timerSeconds);
        lcd.print(" Set   ");
    }
}

void handleTimerButtons(Button btn) {
    if (!timerRunning) {
        if (btn == BTN_UP) {
            timerSeconds += 10;
            if (timerSeconds >= 60) {
                timerSeconds = 0;
                timerMinutes = (timerMinutes + 1) % 100;
            }
        }
        if (btn == BTN_DOWN) {
            timerSeconds -= 10;
            if (timerSeconds < 0) {
                timerSeconds = 50;
                timerMinutes = (timerMinutes + 99) % 100;
            }
        }
        if (btn == BTN_SELECT && (timerMinutes > 0 || timerSeconds > 0)) {
            timerEndTime = millis() + (timerMinutes * 60000UL) + (timerSeconds * 1000UL);
            timerRunning = true;
        }
    } else {
        if (btn == BTN_SELECT || btn == BTN_DOWN) {
            timerRunning = false;
        }
    }
}

void checkTimer() {
    if (!timerRunning) return;

    if (millis() >= timerEndTime) {
        timerRunning = false;
        triggerBuzzer();
    }
}

// ===== BUZZER =====

void triggerBuzzer() {
    for (int i = 0; i < 5; i++) {
        tone(PIN_BUZZER, 1000, 200);
        delay(300);
    }
    noTone(PIN_BUZZER);
}

// ===== MODUS-WECHSEL =====

void handleModeChange(Button btn) {
    if (editMode) return;

    Mode oldMode = currentMode;

    if (btn == BTN_RIGHT) {
        currentMode = (Mode)((currentMode + 1) % 4);
    }
    if (btn == BTN_LEFT) {
        currentMode = (Mode)((currentMode + 3) % 4);
    }

    if (currentMode != oldMode) {
        lcd.clear();
    }
}

// ===== LED-UPDATE =====

void updateLEDs() {
    digitalWrite(PIN_LED_ALARM, alarmEnabled ? HIGH : LOW);
    digitalWrite(PIN_LED_TIMER, timerRunning ? HIGH : LOW);
}

// ===== SETUP =====

void setup() {
    // Button-Pins
    pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_BTN_UP, INPUT_PULLUP);
    pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
    pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
    pinMode(PIN_BTN_SELECT, INPUT_PULLUP);

    // LEDs und Buzzer
    pinMode(PIN_LED_ALARM, OUTPUT);
    pinMode(PIN_LED_TIMER, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);

    // LCD initialisieren
    lcd.init();
    lcd.backlight();
    lcd.print("Multiclock");
    lcd.setCursor(0, 1);
    lcd.print("Starte...");

    // RTC initialisieren
    if (!rtc.begin()) {
        lcd.clear();
        lcd.print("RTC Fehler!");
        while (true);
    }

    if (!rtc.isrunning()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    delay(1000);
    lcd.clear();

    Serial.begin(9600);
    Serial.println("Multiclock gestartet");
}

// ===== HAUPTSCHLEIFE =====

void loop() {
    Button btn = readButton();

    if (btn != BTN_NONE && millis() - lastButtonTime > DEBOUNCE_DELAY) {
        lastButtonTime = millis();

        handleModeChange(btn);

        switch (currentMode) {
            case MODE_CLOCK:     handleClockButtons(btn); break;
            case MODE_STOPWATCH: handleStopwatchButtons(btn); break;
            case MODE_ALARM:     handleAlarmButtons(btn); break;
            case MODE_TIMER:     handleTimerButtons(btn); break;
        }
    }

    // Anzeige aktualisieren
    switch (currentMode) {
        case MODE_CLOCK:     displayClock(); break;
        case MODE_STOPWATCH: displayStopwatch(); break;
        case MODE_ALARM:     displayAlarm(); break;
        case MODE_TIMER:     displayTimer(); break;
    }

    checkAlarm();
    checkTimer();
    updateLEDs();

    delay(50);
}
