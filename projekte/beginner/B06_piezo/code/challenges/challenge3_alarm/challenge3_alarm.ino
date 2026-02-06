// B06 Challenge 3: Lichtalarm
// LDR (Lichtsensor) loest Alarm aus wenn es dunkel wird

const int BUZZER_PIN = 9;   // Piezo an PWM-Pin 9
const int LDR_PIN = A0;     // LDR an Analog-Pin A0
const int LED_PIN = 7;      // Alarm-LED an Pin 7

// Schwellwert fuer Dunkelheit (0-1023)
// Niedriger Wert = wenig Licht = dunkel
const int DUNKEL_SCHWELLE = 300;

// Alarm-Frequenzen
const int ALARM_FREQ_LOW = 800;
const int ALARM_FREQ_HIGH = 1200;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Lichtalarm bereit!");
  Serial.println("Bedecke den LDR fuer Alarm");
}

void loop() {
  // Lichtwert lesen
  int lichtwert = analogRead(LDR_PIN);

  Serial.print("Licht: ");
  Serial.print(lichtwert);

  // Pruefen ob es dunkel ist
  if (lichtwert < DUNKEL_SCHWELLE) {
    Serial.println(" -> ALARM!");

    // Alarm-Sequenz: Wechselnde Toene
    tone(BUZZER_PIN, ALARM_FREQ_HIGH);
    digitalWrite(LED_PIN, HIGH);
    delay(100);

    tone(BUZZER_PIN, ALARM_FREQ_LOW);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  else {
    Serial.println(" -> OK");

    // Kein Alarm
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}
