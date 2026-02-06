# debounce.py - Taster-Entprellung
# Verschiedene Methoden zur Entprellung von Tastern

from machine import Pin
import time

# ============================================
# Methode 1: Einfache Verzögerung
# ============================================

def debounce_simple(pin, delay_ms=50):
    """
    Einfache Entprellung mit Verzögerung.
    Wartet nach Tastendruck eine feste Zeit.
    """
    if pin.value() == 0:  # Taster gedrückt (Pull-up)
        time.sleep_ms(delay_ms)
        if pin.value() == 0:  # Immer noch gedrückt
            return True
    return False

# ============================================
# Methode 2: Zustandsbasiert
# ============================================

class DebouncedButton:
    """
    Zustandsbasierte Entprellung mit konfigurierbarer Verzögerung.
    Erkennt Drücken und Loslassen separat.
    """

    def __init__(self, pin_number, pull_up=True, debounce_ms=50):
        if pull_up:
            self.pin = Pin(pin_number, Pin.IN, Pin.PULL_UP)
            self.pressed_value = 0
        else:
            self.pin = Pin(pin_number, Pin.IN, Pin.PULL_DOWN)
            self.pressed_value = 1

        self.debounce_ms = debounce_ms
        self.last_state = self.pin.value()
        self.last_change_time = time.ticks_ms()
        self.is_pressed = False

    def update(self):
        """
        Muss regelmäßig aufgerufen werden.
        Gibt True zurück bei neuem Tastendruck.
        """
        current_state = self.pin.value()
        current_time = time.ticks_ms()

        # Zustand hat sich geändert
        if current_state != self.last_state:
            self.last_change_time = current_time
            self.last_state = current_state

        # Stabil für debounce_ms?
        if time.ticks_diff(current_time, self.last_change_time) > self.debounce_ms:
            new_pressed = (current_state == self.pressed_value)

            # Flanke erkennen (neu gedrückt)
            if new_pressed and not self.is_pressed:
                self.is_pressed = True
                return True
            elif not new_pressed:
                self.is_pressed = False

        return False

    def is_held(self):
        """Prüft ob Taster gerade gedrückt gehalten wird."""
        return self.is_pressed

# ============================================
# Methode 3: Interrupt-basiert
# ============================================

class InterruptButton:
    """
    Interrupt-basierte Entprellung.
    Reagiert sofort auf Tastendruck.
    """

    def __init__(self, pin_number, callback, pull_up=True, debounce_ms=200):
        if pull_up:
            self.pin = Pin(pin_number, Pin.IN, Pin.PULL_UP)
            trigger = Pin.IRQ_FALLING
        else:
            self.pin = Pin(pin_number, Pin.IN, Pin.PULL_DOWN)
            trigger = Pin.IRQ_RISING

        self.callback = callback
        self.debounce_ms = debounce_ms
        self.last_trigger = 0

        self.pin.irq(trigger=trigger, handler=self._irq_handler)

    def _irq_handler(self, pin):
        current_time = time.ticks_ms()
        if time.ticks_diff(current_time, self.last_trigger) > self.debounce_ms:
            self.last_trigger = current_time
            self.callback()

# ============================================
# Beispiel-Verwendung
# ============================================

if __name__ == "__main__":
    # Methode 2: Zustandsbasiert
    button = DebouncedButton(pin_number=15, pull_up=True)
    led = Pin(25, Pin.OUT)  # Onboard-LED

    print("Drücke den Taster an GPIO15...")

    while True:
        if button.update():
            print("Taster gedrückt!")
            led.toggle()
        time.sleep_ms(10)
