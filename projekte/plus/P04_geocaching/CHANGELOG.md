# P04 ArduinoGeocache - Bugfix, Optimierung & Wokwi-Port

## Ausgangslage

Geocache-Logbuch mit LCD Keypad Shield (parallel LCD + analoge Buttons auf A0).
Original-Datei: `ArduinoGeocache/AruinoGeocache.ino` (Tippfehler im Dateinamen).

---

## Behobene Bugs

### 1. Keine Datenpersistenz
**Problem:** Alle Eintraege gingen bei Reset/Stromverlust verloren.
**Fix:** EEPROM-Speicherung mit Magic Byte (0xCA), `loadFromEEPROM()` in `setup()`, `saveEntryToEEPROM()` + `saveCountToEEPROM()` nach erfolgreichem Eintrag. `EEPROM.update()` statt `write()` schont Schreibzyklen.

**EEPROM-Layout (Arduino Uno: 1024 Bytes):**

| Adresse | Inhalt |
|---------|--------|
| 0 | Magic Byte (0xCA) |
| 1 | entryCount (0-5) |
| 2+ | 5x Entry a 44 Bytes (name[33] + date[11]) |

### 2. Buffer Overflow
**Problem:** `tempName[9]` erlaubte nur 8 Zeichen, aber die Eingabelogik akzeptierte bis zu 32 Zeichen.
**Fix:** `tempName[33]` (32 Zeichen + Nullterminator).

### 3. strncpy-Grenzen falsch
**Problem:** `strncpy(entries[entryCount].name, tempName, 9)` kopierte nur 9 statt 32 Bytes.
**Fix:** `strncpy(..., 32)` mit explizitem Nullterminator `entries[...].name[32] = '\0'`.

### 4. Statische Variablen nicht zurueckgesetzt
**Problem:** `static int pos` in `inputDate()`, `static int viewIndex` in `viewEntries()` und `static char dateChar` behielten Werte zwischen Aufrufen - zweiter Eintrag startete nicht bei Position 0.
**Fix:** Globale Variablen `datePos`, `dateChar`, `viewIndex` mit explizitem Reset in `startNewEntry()` bzw. beim State-Wechsel.

### 5. Variable Shadowing
**Problem:** Lokales `char currentChar` in `inputName()` ueberdeckte globales `currentChar`.
**Fix:** Globales `currentChar` entfernt. Nur `currentCharIndex` + `allowedChars[]` verwendet.

### 6. Keine Eingabevalidierung
**Problem:** Leere Namen konnten gespeichert werden, unvollstaendige Daten ohne Feedback.
**Fix:** Leerer Name wird abgelehnt mit Hinweis "Name ist leer! Bitte eingeben."

### 7. LCD-Flackern
**Problem:** `lcd.clear()` bei jedem Loop-Durchlauf, kein sauberes State-Handling.
**Fix:** `stateChanged`-Flag eingefuehrt. `lcd.clear()` nur bei State-Wechsel. Display-Update in `inputName()` nur nach tatsaechlichem Tastendruck (ausgelagert in `drawNameInput()`).

### 8. "Gespeichert!" bei unvollstaendigem Datum
**Problem:** "Gespeichert!" wurde auch angezeigt, wenn das Datum nicht komplett eingegeben war.
**Fix:** Unterscheidung: "Gespeichert! Eintrag #N" nur bei `datePos == 10`, sonst "Datum fehlt! Bitte komplett."

### 9. Debounce mangelhaft
**Problem:** `delay(20/30)` blockierte die gesamte Ausfuehrung.
**Fix:** `millis()`-basiertes Debouncing mit `DEBOUNCE_DELAY = 200ms` in `readButton()`.

---

## Weitere Optimierungen

- **Dateiname korrigiert:** `AruinoGeocache.ino` -> `ArduinoGeocache.ino`
- **Button-Defines durch Enum ersetzt:** `#define btnRIGHT 0` -> `enum Button { BTN_NONE, BTN_RIGHT, ... }` (konsistent mit P01/P02)
- **Startup-Info:** Zeigt beim Start die Anzahl gespeicherter Eintraege an
- **Eintragsanzeige verbessert:** Zeigt Eintragsnummer (z.B. "2/5") in der rechten Ecke
- **LEFT-Taste in Datumseingabe:** Ermoeglicht Zeichen loeschen (inkl. automatischer Punkte)

---

## Wokwi-Version

Neue Version unter `code/` fuer den Wokwi-Simulator erstellt.

### Dateien
| Datei | Beschreibung |
|-------|-------------|
| `code/code.ino` | I2C LCD + digitale Buttons |
| `code/diagram.json` | Schaltplan (identisch P01) |
| `code/wokwi.toml` | Build-Konfiguration |

### Hardware-Unterschiede zur Original-Version

| Aspekt | Original (Shield) | Wokwi |
|--------|-------------------|-------|
| LCD | `LiquidCrystal` (parallel, Pins 4-9) | `LiquidCrystal_I2C` (I2C, A4/A5) |
| Buttons | `analogRead(A0)` mit Spannungsteiler | `digitalRead()` mit INPUT_PULLUP |
| Button-Pins | alle auf A0 (analog) | D2=RIGHT, D3=UP, D4=DOWN, D5=LEFT, D6=SELECT |
| LCD-Init | `lcd.begin(16, 2)` | `lcd.init(); lcd.backlight();` |

Die gesamte Kernlogik (State Machine, EEPROM, Eingabe, Validierung) ist identisch.

---

## Kompilierung

| Version | Flash | RAM |
|---------|-------|-----|
| Original (parallel LCD) | 6524 B (20%) | 673 B (32%) |
| Wokwi (I2C LCD) | 8114 B (25%) | 885 B (43%) |

Beide Versionen kompilieren fehlerfrei fuer `arduino:avr:uno`.

---

## Kapazitaet

Mit dem aktuellen EEPROM-Layout (2 Bytes Header + 44 Bytes pro Eintrag) passen maximal 23 Eintraege in die 1024 Bytes des Arduino Uno. `MAX_ENTRIES` ist aktuell auf 5 begrenzt.

## Projektstruktur

```
P04_geocaching/
├── CHANGELOG.md              # Diese Datei
├── ArduinoGeocache/
│   └── ArduinoGeocache.ino   # Original (LCD Keypad Shield)
├── code/
│   ├── code.ino              # Wokwi-Version (I2C LCD + Buttons)
│   ├── diagram.json          # Wokwi-Schaltplan
│   └── wokwi.toml            # Wokwi-Build-Konfiguration
└── main/
    └── main.ino              # Aeltere Version (unveraendert)
```
