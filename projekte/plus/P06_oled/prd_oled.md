# Oled Demo


## Oled Display
0,91 Zoll OLED Display I2C SSD1306 Chip 128 x 32 Pixel I2C
OLED_SDA = A4
OLED_SCK = A5

## Quellen
https://www.az-delivery.de/products/0-91-zoll-i2c-oled-display
https://www.berrybase.de/0.91-128x32-oled-display-modul-einfarbig-weiss-i2c-interface?srsltid=AfmBOopb0OPviwdoS4SB9s83RIovS8_qGtPGj65FK2ZNX5liIc0jKxJ6

## weitere Hardware
Arduino Uno R4 (real, Simulator kann abweichen)
const int PIN_TM1637_CLK = 2;
const int PIN_TM1637_DIO = 3;
const int PIN_DHT = 4;  DHT22

## Ziele:

Start: Ausführliche Displaydemo Oled
    - Schriftgrößen
    - Symbole
    - Grafik
    - etc
Dauerhaft:
    Oled:
    - analoge Uhrzeit? 
    - Datum
    - Temperature
    - Humidity
    TM1637:
    - Uhrzeit auf TM1637

## Umsetzung:
- Planen
- Voraussetzungen schaffen
    - Software prüfen / laden
    - Bibliotheken prüfen / laden
- interaktives Prototyping Wokwi in VSCode mit Simulator
- Nach Abschluss des Prototypings auf reale Hardware laden und starten
