# helpers.py - Hilfsfunktionen für MicroPython-Projekte
# Wiederverwendbare Utility-Funktionen

import time

# ============================================
# Wert-Mapping und Skalierung
# ============================================

def map_value(value, in_min, in_max, out_min, out_max):
    """
    Mappt einen Wert von einem Bereich auf einen anderen.
    Entspricht der Arduino map()-Funktion.

    Beispiel:
        # ADC-Wert (0-65535) auf PWM (0-100) mappen
        pwm_value = map_value(adc_value, 0, 65535, 0, 100)
    """
    return int((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

def constrain(value, min_val, max_val):
    """
    Begrenzt einen Wert auf einen Bereich.
    Entspricht der Arduino constrain()-Funktion.
    """
    return max(min_val, min(value, max_val))

def lerp(start, end, t):
    """
    Lineare Interpolation zwischen zwei Werten.
    t=0 gibt start, t=1 gibt end.
    """
    return start + (end - start) * t

# ============================================
# Mittelwertbildung und Glättung
# ============================================

class MovingAverage:
    """
    Gleitender Mittelwert für Sensordaten.
    Reduziert Rauschen bei ADC-Messungen.
    """

    def __init__(self, window_size=10):
        self.window_size = window_size
        self.values = []

    def add(self, value):
        """Fügt einen Wert hinzu und gibt den Mittelwert zurück."""
        self.values.append(value)
        if len(self.values) > self.window_size:
            self.values.pop(0)
        return sum(self.values) / len(self.values)

    def get_average(self):
        """Gibt den aktuellen Mittelwert zurück."""
        if not self.values:
            return 0
        return sum(self.values) / len(self.values)

    def reset(self):
        """Setzt den Filter zurück."""
        self.values = []

# ============================================
# Timing-Helfer
# ============================================

class Timer:
    """
    Nicht-blockierender Timer für periodische Aufgaben.
    Ersetzt time.sleep() in der Hauptschleife.
    """

    def __init__(self, interval_ms):
        self.interval = interval_ms
        self.last_time = time.ticks_ms()

    def is_ready(self):
        """Gibt True zurück wenn das Intervall abgelaufen ist."""
        current = time.ticks_ms()
        if time.ticks_diff(current, self.last_time) >= self.interval:
            self.last_time = current
            return True
        return False

    def set_interval(self, interval_ms):
        """Ändert das Intervall."""
        self.interval = interval_ms

    def reset(self):
        """Setzt den Timer zurück."""
        self.last_time = time.ticks_ms()

class Stopwatch:
    """
    Stoppuhr zum Messen von Zeitintervallen.
    """

    def __init__(self):
        self.start_time = 0
        self.running = False

    def start(self):
        """Startet die Stoppuhr."""
        self.start_time = time.ticks_ms()
        self.running = True

    def stop(self):
        """Stoppt die Stoppuhr und gibt die Zeit in ms zurück."""
        if self.running:
            self.running = False
            return time.ticks_diff(time.ticks_ms(), self.start_time)
        return 0

    def elapsed(self):
        """Gibt die verstrichene Zeit in ms zurück (ohne zu stoppen)."""
        if self.running:
            return time.ticks_diff(time.ticks_ms(), self.start_time)
        return 0

# ============================================
# Formatierung
# ============================================

def format_time(seconds):
    """Formatiert Sekunden als HH:MM:SS."""
    hours = int(seconds // 3600)
    minutes = int((seconds % 3600) // 60)
    secs = int(seconds % 60)
    return f"{hours:02d}:{minutes:02d}:{secs:02d}"

def format_temperature(celsius, decimals=1):
    """Formatiert Temperatur mit Einheit."""
    return f"{celsius:.{decimals}f}°C"

def format_humidity(percent, decimals=1):
    """Formatiert Luftfeuchtigkeit mit Einheit."""
    return f"{percent:.{decimals}f}%"

def format_distance(cm, decimals=1):
    """Formatiert Entfernung mit Einheit."""
    if cm >= 100:
        return f"{cm/100:.{decimals}f}m"
    return f"{cm:.{decimals}f}cm"

# ============================================
# RGB-Farben
# ============================================

def hsv_to_rgb(h, s, v):
    """
    Konvertiert HSV zu RGB.
    h: 0-360, s: 0-100, v: 0-100
    Gibt (r, g, b) mit Werten 0-255 zurück.
    """
    h = h % 360
    s = s / 100
    v = v / 100

    c = v * s
    x = c * (1 - abs((h / 60) % 2 - 1))
    m = v - c

    if h < 60:
        r, g, b = c, x, 0
    elif h < 120:
        r, g, b = x, c, 0
    elif h < 180:
        r, g, b = 0, c, x
    elif h < 240:
        r, g, b = 0, x, c
    elif h < 300:
        r, g, b = x, 0, c
    else:
        r, g, b = c, 0, x

    return (int((r + m) * 255), int((g + m) * 255), int((b + m) * 255))

def wheel(pos):
    """
    Erzeugt Regenbogenfarben für NeoPixel.
    pos: 0-255
    """
    if pos < 85:
        return (255 - pos * 3, pos * 3, 0)
    elif pos < 170:
        pos -= 85
        return (0, 255 - pos * 3, pos * 3)
    else:
        pos -= 170
        return (pos * 3, 0, 255 - pos * 3)

# ============================================
# Beispiel-Verwendung
# ============================================

if __name__ == "__main__":
    # Wert-Mapping testen
    adc_val = 32768  # Mittlerer ADC-Wert
    pwm_val = map_value(adc_val, 0, 65535, 0, 100)
    print(f"ADC {adc_val} -> PWM {pwm_val}%")

    # Moving Average testen
    avg = MovingAverage(5)
    for val in [100, 102, 98, 101, 99, 103, 97]:
        result = avg.add(val)
        print(f"Wert: {val}, Mittelwert: {result:.1f}")

    # Timer testen
    timer = Timer(1000)  # 1 Sekunde
    print("\nTimer-Test (drücke Ctrl+C zum Beenden):")
    count = 0
    while count < 5:
        if timer.is_ready():
            count += 1
            print(f"Tick {count}")
