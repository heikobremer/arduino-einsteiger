// Challenge 1: Automatischer Regenbogen-Farbwechsel
// Die RGB-LED durchlaeuft automatisch alle Regenbogenfarben
// Ohne Potentiometer - nur die RGB-LED

const int LED_R = 9;    // Rote LED an PWM-Pin 9
const int LED_G = 10;   // Gruene LED an PWM-Pin 10
const int LED_B = 11;   // Blaue LED an PWM-Pin 11

int r = 255;  // Startwert Rot
int g = 0;    // Startwert Gruen
int b = 0;    // Startwert Blau

int phase = 0;  // Aktuelle Phase des Farbuebergangs

void setup() {
  // RGB-LED Pins als Ausgang
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  Serial.begin(9600);
  Serial.println("Regenbogen-Farbwechsel gestartet!");
}

void loop() {
  // Regenbogen-Zyklus in 6 Phasen:
  // Phase 0: Rot -> Gelb (Gruen erhoehen)
  // Phase 1: Gelb -> Gruen (Rot verringern)
  // Phase 2: Gruen -> Cyan (Blau erhoehen)
  // Phase 3: Cyan -> Blau (Gruen verringern)
  // Phase 4: Blau -> Magenta (Rot erhoehen)
  // Phase 5: Magenta -> Rot (Blau verringern)

  switch (phase) {
    case 0:  // Rot -> Gelb
      g++;
      if (g >= 255) {
        g = 255;
        phase = 1;
      }
      break;

    case 1:  // Gelb -> Gruen
      r--;
      if (r <= 0) {
        r = 0;
        phase = 2;
      }
      break;

    case 2:  // Gruen -> Cyan
      b++;
      if (b >= 255) {
        b = 255;
        phase = 3;
      }
      break;

    case 3:  // Cyan -> Blau
      g--;
      if (g <= 0) {
        g = 0;
        phase = 4;
      }
      break;

    case 4:  // Blau -> Magenta
      r++;
      if (r >= 255) {
        r = 255;
        phase = 5;
      }
      break;

    case 5:  // Magenta -> Rot
      b--;
      if (b <= 0) {
        b = 0;
        phase = 0;  // Zurueck zum Anfang
      }
      break;
  }

  // RGB-LED steuern
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);

  // Kurze Verzoegerung fuer sanften Uebergang
  delay(10);
}
