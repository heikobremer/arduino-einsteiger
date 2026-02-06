// B06 Challenge 4: Haenschen Klein
// Eine andere Melodie mit dem Piezo spielen

const int BUZZER_PIN = 9;  // Piezo an PWM-Pin 9

// Noten-Frequenzen
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_H4  494
#define NOTE_C5  523
#define NOTE_PAUSE 0

// Melodie: "Haenschen klein"
// Text: "Haenschen klein, ging allein, in die weite Welt hinein..."
int melodie[] = {
  // "Haens-chen klein"
  NOTE_G4, NOTE_E4, NOTE_E4, NOTE_PAUSE,
  // "ging al-lein"
  NOTE_F4, NOTE_D4, NOTE_D4, NOTE_PAUSE,
  // "in die wei-te"
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
  // "Welt hin-ein"
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_PAUSE,

  // "Haens-chen klein"
  NOTE_G4, NOTE_E4, NOTE_E4, NOTE_PAUSE,
  // "ging al-lein"
  NOTE_F4, NOTE_D4, NOTE_D4, NOTE_PAUSE,
  // "in die wei-te"
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_G4,
  // "Welt hin-ein"
  NOTE_C4, NOTE_PAUSE
};

// Notendauer in ms
int dauer[] = {
  400, 400, 800, 200,
  400, 400, 800, 200,
  400, 400, 400, 400,
  400, 400, 800, 200,

  400, 400, 800, 200,
  400, 400, 800, 200,
  400, 400, 400, 400,
  1200, 200
};

int anzahlNoten = sizeof(melodie) / sizeof(melodie[0]);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Spiele: Haenschen klein");

  // Melodie einmal abspielen
  spieleMelodie();

  Serial.println("Fertig!");
}

void spieleMelodie() {
  for (int i = 0; i < anzahlNoten; i++) {
    int note = melodie[i];
    int noteDauer = dauer[i];

    if (note == NOTE_PAUSE) {
      // Pause - kein Ton
      noTone(BUZZER_PIN);
    } else {
      // Note spielen
      tone(BUZZER_PIN, note, noteDauer);
      Serial.print("Note ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(note);
      Serial.println(" Hz");
    }

    // Warten bis Note fertig + kurze Pause
    delay(noteDauer + 50);
    noTone(BUZZER_PIN);
  }
}

void loop() {
  // Melodie wird nur einmal in setup() gespielt
  // Fuer Wiederholung: Reset druecken
  // Oder: spieleMelodie() hier aufrufen mit delay
}
