// A05: Memory Game (Simon Says)
// 4 LEDs + 4 Buttons + Piezo - Sequenz merken und nachspielen

// Pin-Definitionen
const int LED_PINS[] = {2, 3, 4, 5};      // 4 LEDs
const int BTN_PINS[] = {6, 7, 8, 9};      // 4 Buttons (INPUT_PULLUP)
const int PIEZO_PIN = 10;                  // Piezo-Summer

// Töne für jede Farbe (Hz)
const int TONES[] = {262, 330, 392, 523};  // C, E, G, C (Oktave höher)

// Spielkonfiguration
const int MAX_LEVEL = 20;                  // Maximale Sequenzlänge
const int SHOW_DELAY = 500;                // ms pro LED bei Anzeige
const int PAUSE_DELAY = 200;               // ms Pause zwischen LEDs

// Spielvariablen
int sequence[MAX_LEVEL];                   // Gespeicherte Sequenz
int currentLevel = 0;                      // Aktuelles Level (Sequenzlänge)
int playerPosition = 0;                    // Spielerposition in Sequenz
bool gameOver = false;

// Entprellung
unsigned long lastButtonTime = 0;
const unsigned long DEBOUNCE_DELAY = 150;

void setup() {
  Serial.begin(9600);

  // LEDs als Output
  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  // Buttons als Input mit Pull-up
  for (int i = 0; i < 4; i++) {
    pinMode(BTN_PINS[i], INPUT_PULLUP);
  }

  // Piezo
  pinMode(PIEZO_PIN, OUTPUT);

  // Zufallsgenerator initialisieren
  randomSeed(analogRead(A0));

  Serial.println(F("==================="));
  Serial.println(F("  MEMORY GAME"));
  Serial.println(F("  (Simon Says)"));
  Serial.println(F("==================="));
  Serial.println(F("Druecke einen Taster zum Starten!"));

  // Startanimation
  startAnimation();

  // Warten auf Tastendruck zum Starten
  waitForAnyButton();

  // Neues Spiel starten
  startNewGame();
}

void startAnimation() {
  // LEDs nacheinander einschalten
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    tone(PIEZO_PIN, TONES[i], 100);
    delay(150);
  }
  delay(200);
  // Alle aus
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
}

void waitForAnyButton() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(BTN_PINS[i]) == LOW) {
        delay(200);  // Entprellung
        return;
      }
    }
  }
}

void startNewGame() {
  currentLevel = 0;
  playerPosition = 0;
  gameOver = false;

  Serial.println(F("\nNeues Spiel gestartet!"));
  delay(1000);

  // Erstes Element zur Sequenz hinzufügen und anzeigen
  addToSequence();
  showSequence();
}

void addToSequence() {
  if (currentLevel < MAX_LEVEL) {
    sequence[currentLevel] = random(0, 4);
    currentLevel++;
    Serial.print(F("Level: "));
    Serial.println(currentLevel);
  }
}

void showSequence() {
  Serial.println(F("Merke dir die Sequenz:"));

  delay(500);  // Kurze Pause vor Anzeige

  for (int i = 0; i < currentLevel; i++) {
    int led = sequence[i];

    // LED einschalten und Ton spielen
    digitalWrite(LED_PINS[led], HIGH);
    tone(PIEZO_PIN, TONES[led], SHOW_DELAY - 50);
    delay(SHOW_DELAY);

    // LED ausschalten
    digitalWrite(LED_PINS[led], LOW);
    delay(PAUSE_DELAY);
  }

  Serial.println(F("Dein Zug!"));
  playerPosition = 0;
}

void playTone(int index, int duration) {
  tone(PIEZO_PIN, TONES[index], duration);
}

void errorSound() {
  // Trauriger Ton bei Fehler
  tone(PIEZO_PIN, 200, 200);
  delay(250);
  tone(PIEZO_PIN, 150, 400);
}

void winSound() {
  // Siegesmelodie
  for (int i = 0; i < 4; i++) {
    tone(PIEZO_PIN, TONES[i], 100);
    delay(150);
  }
  tone(PIEZO_PIN, TONES[3] * 2, 300);
}

void flashAllLEDs(int times) {
  for (int t = 0; t < times; t++) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(LED_PINS[i], HIGH);
    }
    delay(200);
    for (int i = 0; i < 4; i++) {
      digitalWrite(LED_PINS[i], LOW);
    }
    delay(200);
  }
}

int getButtonPress() {
  // Prüft ob ein Button gedrückt wurde und gibt Index zurück (-1 wenn keiner)
  for (int i = 0; i < 4; i++) {
    if (digitalRead(BTN_PINS[i]) == LOW) {
      // LED einschalten und Ton spielen
      digitalWrite(LED_PINS[i], HIGH);
      playTone(i, 150);

      // Warten bis losgelassen
      while (digitalRead(BTN_PINS[i]) == LOW) {
        delay(10);
      }

      // LED ausschalten
      digitalWrite(LED_PINS[i], LOW);
      delay(50);  // Kleine Pause

      return i;
    }
  }
  return -1;
}

void handlePlayerInput(int buttonIndex) {
  if (buttonIndex == sequence[playerPosition]) {
    // Richtig!
    playerPosition++;

    if (playerPosition >= currentLevel) {
      // Level geschafft!
      Serial.println(F("Richtig!"));
      delay(500);

      if (currentLevel >= MAX_LEVEL) {
        // Spiel gewonnen!
        Serial.println(F("\n*** GEWONNEN! ***"));
        Serial.println(F("Du hast alle Level geschafft!"));
        winSound();
        flashAllLEDs(5);
        gameOver = true;
      } else {
        // Nächstes Level
        addToSequence();
        showSequence();
      }
    }
  } else {
    // Falsch!
    Serial.println(F("\n*** FALSCH! ***"));
    Serial.print(F("Erreicht: Level "));
    Serial.println(currentLevel);
    errorSound();
    flashAllLEDs(3);
    gameOver = true;
  }
}

void loop() {
  if (gameOver) {
    Serial.println(F("\nTaster druecken fuer neues Spiel..."));
    waitForAnyButton();
    startNewGame();
    return;
  }

  // Auf Spielereingabe warten
  int button = getButtonPress();

  if (button >= 0) {
    handlePlayerInput(button);
  }
}
