// Challenge 3: Dunkelheits-Alarm mit Buzzer
// Piezo-Buzzer piept wenn es zu dunkel wird

const int LDR_PIN = A0;       // Lichtsensor an A0
const int BUZZER_PIN = 8;     // Piezo-Buzzer an Pin 8
const int LED_PIN = 9;        // Warn-LED an Pin 9

// Schwellwert fuer Alarm (Dunkelheit)
const int ALARM_SCHWELLE = 700;

// Alarmton-Frequenz in Hz
const int ALARM_TON = 1000;

// Zustand merken (um nicht staendig zu piepen)
bool alarmAktiv = false;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Dunkelheits-Alarm gestartet!");
  Serial.println("Buzzer piept bei Dunkelheit.");
}

void loop() {
  // LDR auslesen
  int wert = analogRead(LDR_PIN);

  if (wert > ALARM_SCHWELLE) {
    // Zu dunkel - Alarm ausloesen!
    if (!alarmAktiv) {
      Serial.println("!!! ALARM: Zu dunkel !!!");
      alarmAktiv = true;
    }

    // LED blinken und Buzzer piepen
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, ALARM_TON, 100);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);

  } else {
    // Genug Licht - kein Alarm
    if (alarmAktiv) {
      Serial.println("Alarm beendet - genug Licht.");
      alarmAktiv = false;
    }

    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);

    // Normaler Status
    Serial.print("LDR: ");
    Serial.print(wert);
    Serial.println(" - OK");

    delay(300);
  }
}
