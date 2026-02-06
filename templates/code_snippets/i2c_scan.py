# i2c_scan.py - I2C-Geräte finden
# Hilfsprogramm zum Erkennen von I2C-Adressen

from machine import I2C, Pin
import time

def scan_i2c(i2c_id=0, sda_pin=0, scl_pin=1, freq=400000):
    """
    Scannt den I2C-Bus nach verbundenen Geräten.

    Parameter:
    - i2c_id: I2C-Bus-Nummer (0 oder 1)
    - sda_pin: GPIO-Pin für SDA
    - scl_pin: GPIO-Pin für SCL
    - freq: I2C-Frequenz in Hz

    Rückgabe: Liste der gefundenen Adressen
    """
    print(f"Scanne I2C-Bus {i2c_id}...")
    print(f"SDA: GPIO{sda_pin}, SCL: GPIO{scl_pin}")
    print("-" * 40)

    i2c = I2C(i2c_id, sda=Pin(sda_pin), scl=Pin(scl_pin), freq=freq)
    devices = i2c.scan()

    if devices:
        print(f"Gefundene Geräte: {len(devices)}")
        print()
        for addr in devices:
            print(f"  Adresse: {addr} (0x{addr:02X})")
            # Bekannte Adressen identifizieren
            device_name = identify_device(addr)
            if device_name:
                print(f"           -> Möglicherweise: {device_name}")
    else:
        print("Keine I2C-Geräte gefunden!")
        print()
        print("Überprüfe:")
        print("  - Sind die Kabel richtig verbunden?")
        print("  - Stimmen die Pin-Nummern?")
        print("  - Ist das Gerät mit Strom versorgt?")

    print("-" * 40)
    return devices

def identify_device(address):
    """Identifiziert bekannte I2C-Geräte anhand ihrer Adresse."""
    known_devices = {
        0x27: "LCD 1602 (PCF8574)",
        0x3F: "LCD 1602 (PCF8574A)",
        0x3C: "SSD1306 OLED Display",
        0x3D: "SSD1306 OLED Display (Alternativ)",
        0x40: "INA219 Stromsensor / PCA9685 PWM",
        0x48: "ADS1115 ADC / PCF8591 ADC",
        0x50: "AT24C32 EEPROM",
        0x57: "DS3231 EEPROM",
        0x68: "DS3231 RTC / MPU6050 IMU",
        0x76: "BMP280 / BME280 Drucksensor",
        0x77: "BMP280 / BME280 (Alternativ)",
        0x23: "BH1750 Lichtsensor",
        0x29: "VL53L0X ToF Sensor",
        0x38: "AHT20 Temperatur/Feuchtigkeit",
        0x44: "SHT30/SHT31 Temperatur/Feuchtigkeit"
    }
    return known_devices.get(address)

# ============================================
# Hauptprogramm
# ============================================

if __name__ == "__main__":
    # Raspberry Pi Pico Standard-Pins
    scan_i2c(i2c_id=0, sda_pin=0, scl_pin=1)

    # Optional: Zweiten I2C-Bus scannen
    # scan_i2c(i2c_id=1, sda_pin=2, scl_pin=3)
