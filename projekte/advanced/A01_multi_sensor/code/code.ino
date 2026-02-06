// A01: Multi-Sensor Dashboard
// DHT22 + LCD 16x2 I2C + 2 Status-LEDs

#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// Pin-Definitionen
const int DHT_PIN = 7;        // DHT22 Datenpin
const int LED_GRUEN = 8;      // Komfortzone (18-25°C)
const int LED_ROT = 9;        // Außerhalb Komfortzone

// DHT Sensor
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

// LCD Display (I2C Adresse 0x27, 16 Zeichen, 2 Zeilen)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Komfortzone
const float TEMP_MIN = 18.0;
const float TEMP_MAX = 25.0;

// Variablen
float temperatur = 0;
float luftfeuchtigkeit = 0;
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 2000;  // 2 Sekunden

// Custom Zeichen für Grad-Symbol
byte gradSymbol[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  Serial.begin(9600);

  // Pins konfigurieren
  pinMode(LED_GRUEN, OUTPUT);
  pinMode(LED_ROT, OUTPUT);

  // DHT initialisieren
  dht.begin();

  // LCD initialisieren
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, gradSymbol);

  // Startbildschirm
  lcd.setCursor(0, 0);
  lcd.print("Multi-Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Dashboard v1.0");

  Serial.println(F("Multi-Sensor Dashboard"));
  Serial.println(F("======================"));
  Serial.print(F("Komfortzone: "));
  Serial.print(TEMP_MIN);
  Serial.print(F("-"));
  Serial.print(TEMP_MAX);
  Serial.println(F(" C"));
  Serial.println(F("======================"));

  delay(2000);
  lcd.clear();
}

void updateLEDs(float temp) {
  if (temp >= TEMP_MIN && temp <= TEMP_MAX) {
    // Komfortzone - grüne LED an
    digitalWrite(LED_GRUEN, HIGH);
    digitalWrite(LED_ROT, LOW);
  } else {
    // Außerhalb - rote LED an
    digitalWrite(LED_GRUEN, LOW);
    digitalWrite(LED_ROT, HIGH);
  }
}

void updateDisplay() {
  // Zeile 1: Temperatur
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");

  if (isnan(temperatur)) {
    lcd.print("Fehler  ");
  } else {
    lcd.print(temperatur, 1);
    lcd.write(0);  // Grad-Symbol
    lcd.print("C   ");
  }

  // Zeile 2: Luftfeuchtigkeit
  lcd.setCursor(0, 1);
  lcd.print("Feuchte: ");

  if (isnan(luftfeuchtigkeit)) {
    lcd.print("Fehler");
  } else {
    lcd.print(luftfeuchtigkeit, 0);
    lcd.print("%  ");
  }
}

void loop() {
  unsigned long currentTime = millis();

  // Alle UPDATE_INTERVAL ms Sensoren auslesen
  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = currentTime;

    // Sensoren auslesen
    luftfeuchtigkeit = dht.readHumidity();
    temperatur = dht.readTemperature();

    // Prüfen ob Lesefehler
    if (isnan(luftfeuchtigkeit) || isnan(temperatur)) {
      Serial.println(F("Fehler beim Lesen des DHT22!"));
      // Beide LEDs aus bei Fehler
      digitalWrite(LED_GRUEN, LOW);
      digitalWrite(LED_ROT, LOW);
    } else {
      // LEDs aktualisieren
      updateLEDs(temperatur);

      // Serial Ausgabe
      Serial.print(F("Temperatur: "));
      Serial.print(temperatur);
      Serial.print(F(" C | Luftfeuchtigkeit: "));
      Serial.print(luftfeuchtigkeit);
      Serial.print(F("% | "));

      if (temperatur >= TEMP_MIN && temperatur <= TEMP_MAX) {
        Serial.println(F("KOMFORTZONE"));
      } else if (temperatur < TEMP_MIN) {
        Serial.println(F("ZU KALT"));
      } else {
        Serial.println(F("ZU WARM"));
      }
    }

    // Display aktualisieren
    updateDisplay();
  }
}
