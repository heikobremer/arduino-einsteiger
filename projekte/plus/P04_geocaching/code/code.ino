// P04: Arduino Geocache - Logbuch (Wokwi-Version)
// I2C LCD mit 5 digitalen Buttons
// Eintraege werden im EEPROM gespeichert und ueberleben einen Reset

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

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

// ===== DEBOUNCE =====
const unsigned long DEBOUNCE_DELAY = 200;
unsigned long lastButtonTime = 0;

// ===== LCD-OBJEKT (I2C Adresse 0x27, 16 Zeichen, 2 Zeilen) =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Button-Lesen ueber digitale Pins (INPUT_PULLUP, LOW-aktiv)
Button readButton() {
  // Debounce: zu fruehe Abfrage ignorieren
  if (millis() - lastButtonTime < DEBOUNCE_DELAY) return BTN_NONE;

  Button btn = BTN_NONE;
  if (digitalRead(PIN_BTN_RIGHT) == LOW)  btn = BTN_RIGHT;
  else if (digitalRead(PIN_BTN_UP) == LOW)     btn = BTN_UP;
  else if (digitalRead(PIN_BTN_DOWN) == LOW)   btn = BTN_DOWN;
  else if (digitalRead(PIN_BTN_LEFT) == LOW)   btn = BTN_LEFT;
  else if (digitalRead(PIN_BTN_SELECT) == LOW) btn = BTN_SELECT;

  if (btn != BTN_NONE) {
    lastButtonTime = millis();
  }
  return btn;
}

// ===== GEOCACHE-EINTRAG =====
struct Entry {
  char name[33];  // Max 32 Zeichen + Nullterminator
  char date[11];  // Format: DD.MM.YYYY + Nullterminator
};

#define MAX_ENTRIES 5
Entry entries[MAX_ENTRIES];
int entryCount = 0;

// ===== EEPROM-LAYOUT =====
// Adresse 0:   Magic Byte (0xCA)
// Adresse 1:   entryCount (0-5)
// Adresse 2+:  5x Entry a 44 Bytes (name[33] + date[11])
#define EEPROM_MAGIC      0xCA
#define EEPROM_ADDR_MAGIC 0
#define EEPROM_ADDR_COUNT 1
#define EEPROM_ADDR_DATA  2
#define ENTRY_SIZE        44  // sizeof(Entry) = 33 + 11

void loadFromEEPROM() {
  if (EEPROM.read(EEPROM_ADDR_MAGIC) != EEPROM_MAGIC) {
    // Erster Start: EEPROM initialisieren
    EEPROM.update(EEPROM_ADDR_MAGIC, EEPROM_MAGIC);
    EEPROM.update(EEPROM_ADDR_COUNT, 0);
    entryCount = 0;
    return;
  }
  entryCount = EEPROM.read(EEPROM_ADDR_COUNT);
  if (entryCount > MAX_ENTRIES) entryCount = MAX_ENTRIES;

  for (int i = 0; i < entryCount; i++) {
    int addr = EEPROM_ADDR_DATA + i * ENTRY_SIZE;
    for (int j = 0; j < 33; j++) {
      entries[i].name[j] = EEPROM.read(addr + j);
    }
    for (int j = 0; j < 11; j++) {
      entries[i].date[j] = EEPROM.read(addr + 33 + j);
    }
  }
}

void saveEntryToEEPROM(int index) {
  int addr = EEPROM_ADDR_DATA + index * ENTRY_SIZE;
  for (int j = 0; j < 33; j++) {
    EEPROM.update(addr + j, entries[index].name[j]);
  }
  for (int j = 0; j < 11; j++) {
    EEPROM.update(addr + 33 + j, entries[index].date[j]);
  }
}

void saveCountToEEPROM() {
  EEPROM.update(EEPROM_ADDR_COUNT, entryCount);
}

// ===== ZUSTAENDE =====
enum State { MAIN_MENU, NEW_ENTRY, INPUT_NAME, INPUT_DATE, VIEW_ENTRIES };
State currentState = MAIN_MENU;
bool stateChanged = true;  // Flag fuer LCD-Neuzeichnung

// ===== ERLAUBTE ZEICHEN =====
const char allowedChars[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const int numAllowedChars = sizeof(allowedChars) - 1;
int currentCharIndex = 1;  // Beginnt bei 'A'

// ===== GLOBALE EINGABEVARIABLEN =====
int menuIndex = 0;
int charPos = 0;
char tempName[33];
char tempDate[11];

// Datumseingabe
int datePos = 0;
char dateChar = '0';

// Eintragsanzeige
int viewIndex = 0;

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

  // EEPROM laden
  loadFromEEPROM();

  lcd.setCursor(0, 0);
  lcd.print("Stefans Cache");
  lcd.setCursor(0, 1);
  lcd.print(entryCount);
  lcd.print(" Eintraege");
  delay(1500);
  lcd.clear();
  stateChanged = true;
}

// ===== HAUPTSCHLEIFE =====
void loop() {
  switch (currentState) {
    case MAIN_MENU:    showMainMenu();   break;
    case NEW_ENTRY:    startNewEntry();  break;
    case INPUT_NAME:   inputName();      break;
    case INPUT_DATE:   inputDate();      break;
    case VIEW_ENTRIES: viewEntries();    break;
  }
}

// ===== HAUPTMENUE =====
void showMainMenu() {
  if (stateChanged) {
    lcd.clear();
    stateChanged = false;
  }

  lcd.setCursor(0, 0);
  if (menuIndex == 0) lcd.print("> Neuer Eintrag ");
  else                lcd.print("  Neuer Eintrag ");

  lcd.setCursor(0, 1);
  if (menuIndex == 1) lcd.print("> Eintraege     ");
  else                lcd.print("  Eintraege     ");

  Button btn = readButton();
  if (btn == BTN_UP) {
    menuIndex = 0;
  } else if (btn == BTN_DOWN) {
    menuIndex = 1;
  } else if (btn == BTN_SELECT) {
    if (menuIndex == 0) {
      currentState = NEW_ENTRY;
    } else {
      viewIndex = 0;  // Reset bei Neuaufruf
      currentState = VIEW_ENTRIES;
    }
    stateChanged = true;
  }
}

// ===== NEUER EINTRAG STARTEN =====
void startNewEntry() {
  if (entryCount >= MAX_ENTRIES) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Max erreicht!");
    lcd.setCursor(0, 1);
    lcd.print("(5 Eintraege)");
    delay(1500);
    currentState = MAIN_MENU;
    stateChanged = true;
    return;
  }
  memset(tempName, 0, sizeof(tempName));
  memset(tempDate, 0, sizeof(tempDate));
  charPos = 0;
  currentCharIndex = 1;  // 'A'
  datePos = 0;
  dateChar = '0';
  currentState = INPUT_NAME;
  stateChanged = true;
}

// ===== NAMENSEINGABE =====
void drawNameInput() {
  lcd.setCursor(0, 0);
  lcd.print("Name eingeben:  ");

  lcd.setCursor(0, 1);
  if (charPos < 16) {
    for (int i = 0; i < charPos; i++) {
      lcd.print(tempName[i]);
    }
    lcd.print(allowedChars[currentCharIndex]);
    for (int i = charPos + 1; i < 16; i++) {
      lcd.print(' ');
    }
  } else {
    for (int i = charPos - 15; i < charPos; i++) {
      lcd.print(tempName[i]);
    }
    lcd.print(allowedChars[currentCharIndex]);
  }

  // Cursor blinken lassen an aktueller Position
  int cursorPos = (charPos < 16) ? charPos : 15;
  lcd.setCursor(cursorPos, 1);
  lcd.blink();
}

void inputName() {
  if (stateChanged) {
    lcd.clear();
    drawNameInput();
    stateChanged = false;
  }

  Button btn = readButton();
  if (btn == BTN_NONE) return;

  if (btn == BTN_UP) {
    currentCharIndex = (currentCharIndex + 1) % numAllowedChars;
  } else if (btn == BTN_DOWN) {
    currentCharIndex = (currentCharIndex - 1 + numAllowedChars) % numAllowedChars;
  } else if (btn == BTN_RIGHT) {
    if (charPos < 32) {
      tempName[charPos] = allowedChars[currentCharIndex];
      charPos++;
      tempName[charPos] = '\0';
      currentCharIndex = 1;  // Zurueck auf 'A'
    }
  } else if (btn == BTN_LEFT) {
    if (charPos > 0) {
      charPos--;
      tempName[charPos] = '\0';
    }
  } else if (btn == BTN_SELECT) {
    lcd.noBlink();
    // Validierung: Name darf nicht leer sein
    if (charPos == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Name ist leer!");
      lcd.setCursor(0, 1);
      lcd.print("Bitte eingeben.");
      delay(1500);
      stateChanged = true;
      return;
    }
    currentState = INPUT_DATE;
    stateChanged = true;
    return;
  }

  // Display nur nach Tastendruck aktualisieren
  drawNameInput();
}

// ===== DATUMSEINGABE =====
void inputDate() {
  if (stateChanged) {
    lcd.clear();
    stateChanged = false;
  }

  lcd.setCursor(0, 0);
  lcd.print("Datum:TT.MM.JJJJ");

  lcd.setCursor(0, 1);
  // Bisherige Eingabe anzeigen
  for (int i = 0; i < datePos; i++) {
    lcd.print(tempDate[i]);
  }
  // Aktuelles Zeichen anzeigen
  if (datePos < 10) {
    lcd.print(dateChar);
    // Rest mit Leerzeichen fuellen
    for (int i = datePos + 1; i < 10; i++) {
      lcd.print(' ');
    }
  }

  Button btn = readButton();
  if (btn == BTN_UP) {
    dateChar++;
    if (dateChar > '9') dateChar = '0';
  } else if (btn == BTN_DOWN) {
    dateChar--;
    if (dateChar < '0') dateChar = '9';
  } else if (btn == BTN_RIGHT) {
    // Automatische Punkte an Positionen 2 und 5
    if (datePos == 2 || datePos == 5) {
      tempDate[datePos] = '.';
      datePos++;
    }
    if (datePos < 10) {
      tempDate[datePos] = dateChar;
      datePos++;
      tempDate[datePos] = '\0';
      dateChar = '0';
    }
  } else if (btn == BTN_LEFT) {
    // Zeichen loeschen
    if (datePos > 0) {
      datePos--;
      // Punkt davor auch loeschen
      if (datePos > 0 && tempDate[datePos - 1] == '.') {
        datePos--;
      }
      tempDate[datePos] = '\0';
    }
  } else if (btn == BTN_SELECT) {
    if (datePos == 10) {
      // Datum komplett - speichern
      strncpy(entries[entryCount].name, tempName, 32);
      entries[entryCount].name[32] = '\0';
      strncpy(entries[entryCount].date, tempDate, 10);
      entries[entryCount].date[10] = '\0';

      saveEntryToEEPROM(entryCount);
      entryCount++;
      saveCountToEEPROM();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gespeichert!");
      lcd.setCursor(0, 1);
      lcd.print("Eintrag #");
      lcd.print(entryCount);
      delay(1500);
    } else {
      // Datum unvollstaendig
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Datum fehlt!");
      lcd.setCursor(0, 1);
      lcd.print("Bitte komplett.");
      delay(1500);
    }
    currentState = MAIN_MENU;
    stateChanged = true;
  }
}

// ===== EINTRAEGE ANZEIGEN =====
void viewEntries() {
  if (stateChanged) {
    lcd.clear();
    stateChanged = false;
  }

  lcd.setCursor(0, 0);
  if (entryCount == 0) {
    lcd.print("Keine Eintraege ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  } else {
    // Name anzeigen (max 16 Zeichen auf LCD)
    char line[17];
    snprintf(line, sizeof(line), "%-16s", entries[viewIndex].name);
    lcd.print(line);

    lcd.setCursor(0, 1);
    lcd.print(entries[viewIndex].date);
    // Eintragsnummer rechts anzeigen
    lcd.setCursor(13, 1);
    lcd.print(viewIndex + 1);
    lcd.print("/");
    lcd.print(entryCount);
  }

  Button btn = readButton();
  if (btn == BTN_LEFT) {
    if (entryCount > 0) {
      viewIndex = (viewIndex - 1 + entryCount) % entryCount;
      stateChanged = true;
    }
  } else if (btn == BTN_RIGHT) {
    if (entryCount > 0) {
      viewIndex = (viewIndex + 1) % entryCount;
      stateChanged = true;
    }
  } else if (btn == BTN_SELECT) {
    currentState = MAIN_MENU;
    stateChanged = true;
  }
}
