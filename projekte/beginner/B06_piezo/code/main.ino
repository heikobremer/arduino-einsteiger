// B06: Piezo-Summer - Fertige Version
// Melodie "Alle meine Entchen" spielen

const int BUZZER_PIN = 9;  // Piezo an PWM-Pin 9

// Noten-Frequenzen
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_PAUSE 0

// Melodie: "Alle meine Entchen"
int melodie[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4,
  NOTE_G4, NOTE_PAUSE,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4,
  NOTE_G4, NOTE_PAUSE,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_C4
};

// Notendauer in ms
int dauer[] = {
  300, 300, 300, 300,
  600, 600,
  300, 300, 300, 300,
  1200, 300,
  300, 300, 300, 300,
  1200, 300,
  300, 300, 300, 300,
  600, 600,
  300, 300, 300, 300,
  1200
};

int anzahlNoten = sizeof(melodie) / sizeof(melodie[0]);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Spiele: Alle meine Entchen");

  // Melodie einmal abspielen
  for (int i = 0; i < anzahlNoten; i++) {
    int note = melodie[i];
    int noteDauer = dauer[i];

    if (note == NOTE_PAUSE) {
      noTone(BUZZER_PIN);
    } else {
      tone(BUZZER_PIN, note, noteDauer);
      Serial.print("Note: ");
      Serial.println(note);
    }

    // Warten bis Note fertig + kurze Pause
    delay(noteDauer + 30);
    noTone(BUZZER_PIN);
  }

  Serial.println("Fertig!");
}

void loop() {
  // Nichts - Melodie wird nur einmal in setup() gespielt
  // Fuer Wiederholung: Code in loop() verschieben
}
