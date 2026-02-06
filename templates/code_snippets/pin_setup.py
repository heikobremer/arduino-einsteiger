# pin_setup.py - GPIO-Konfiguration für MicroPython
# Wiederverwendbares Modul für Pin-Initialisierung

from machine import Pin, PWM, ADC

# ============================================
# LED-Konfiguration
# ============================================

def setup_led(pin_number):
    """Konfiguriert einen Pin als LED-Ausgang."""
    return Pin(pin_number, Pin.OUT)

def setup_led_pwm(pin_number, freq=1000):
    """Konfiguriert einen Pin als PWM für LED-Dimmen."""
    pwm = PWM(Pin(pin_number))
    pwm.freq(freq)
    return pwm

# ============================================
# Taster-Konfiguration
# ============================================

def setup_button(pin_number, pull_up=True):
    """Konfiguriert einen Pin als Taster-Eingang."""
    if pull_up:
        return Pin(pin_number, Pin.IN, Pin.PULL_UP)
    else:
        return Pin(pin_number, Pin.IN, Pin.PULL_DOWN)

# ============================================
# Analog-Eingänge (ADC)
# ============================================

def setup_adc(pin_number):
    """Konfiguriert einen Pin als ADC-Eingang."""
    return ADC(Pin(pin_number))

# ============================================
# Standard-Pin-Definitionen für Raspberry Pi Pico
# ============================================

PICO_PINS = {
    'onboard_led': 25,
    'i2c0_sda': 0,
    'i2c0_scl': 1,
    'i2c1_sda': 2,
    'i2c1_scl': 3,
    'spi0_rx': 16,
    'spi0_cs': 17,
    'spi0_sck': 18,
    'spi0_tx': 19,
    'adc0': 26,
    'adc1': 27,
    'adc2': 28
}

# ============================================
# Standard-Pin-Definitionen für ESP32
# ============================================

ESP32_PINS = {
    'onboard_led': 2,
    'i2c_sda': 21,
    'i2c_scl': 22,
    'spi_mosi': 23,
    'spi_miso': 19,
    'spi_sck': 18,
    'adc1_0': 36,
    'adc1_1': 37,
    'adc1_2': 38,
    'adc1_3': 39
}
