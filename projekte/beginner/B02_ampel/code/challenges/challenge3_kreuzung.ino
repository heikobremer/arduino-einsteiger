// B02 Challenge 3: Kreuzung
// Zwei Ampeln synchronisiert (wenn eine Rot, ist andere Gruen)

// Ampel 1 (Nord-Sued)
const int A1_ROT = 7;
const int A1_GELB = 6;
const int A1_GRUEN = 5;

// Ampel 2 (Ost-West)
const int A2_ROT = 4;
const int A2_GELB = 3;
const int A2_GRUEN = 2;

void ampel1(int rot, int gelb, int gruen) {
  digitalWrite(A1_ROT, rot);
  digitalWrite(A1_GELB, gelb);
  digitalWrite(A1_GRUEN, gruen);
}

void ampel2(int rot, int gelb, int gruen) {
  digitalWrite(A2_ROT, rot);
  digitalWrite(A2_GELB, gelb);
  digitalWrite(A2_GRUEN, gruen);
}

void setup() {
  pinMode(A1_ROT, OUTPUT);
  pinMode(A1_GELB, OUTPUT);
  pinMode(A1_GRUEN, OUTPUT);
  pinMode(A2_ROT, OUTPUT);
  pinMode(A2_GELB, OUTPUT);
  pinMode(A2_GRUEN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Kreuzung mit zwei Ampeln!");
}

void loop() {
  // Phase 1: Ampel 1 Gruen, Ampel 2 Rot
  Serial.println("Nord-Sued faehrt");
  ampel1(LOW, LOW, HIGH);
  ampel2(HIGH, LOW, LOW);
  delay(3000);

  // Phase 2: Ampel 1 Gelb, Ampel 2 Rot
  ampel1(LOW, HIGH, LOW);
  delay(1000);

  // Phase 3: Beide Rot (Raeumzeit)
  ampel1(HIGH, LOW, LOW);
  delay(500);

  // Phase 4: Ampel 1 Rot+Gelb, Ampel 2 Rot
  ampel2(HIGH, HIGH, LOW);
  delay(1000);

  // Phase 5: Ampel 1 Rot, Ampel 2 Gruen
  Serial.println("Ost-West faehrt");
  ampel1(HIGH, LOW, LOW);
  ampel2(LOW, LOW, HIGH);
  delay(3000);

  // Phase 6: Ampel 2 Gelb
  ampel2(LOW, HIGH, LOW);
  delay(1000);

  // Phase 7: Beide Rot (Raeumzeit)
  ampel2(HIGH, LOW, LOW);
  delay(500);

  // Phase 8: Ampel 1 Rot+Gelb
  ampel1(HIGH, HIGH, LOW);
  delay(1000);
}
