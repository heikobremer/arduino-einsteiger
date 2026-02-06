# Präsentations-Framework

Ein minimalistisches, Apple-inspiriertes Präsentations-Framework mit HTML, CSS und JavaScript.

## 📁 Dateien

- **`template.html`** - Muster-Präsentation mit Beispiel-Slides
- **`presentation.html`** - FriendsMovie iOS Training Präsentation
- **`styles.css`** - Alle Styles (wiederverwendbar)
- **`script.js`** - Navigation und Interaktion

## 🎨 Design-Prinzipien

- **Minimalistisch**: Dezente Farben, klare Hierarchie
- **Apple-like**: Dunkler Hintergrund, weiße Slides mit Schatten
- **Professionell**: Grau-Palette mit blauen Akzenten (#0071e3)
- **Responsive**: Funktioniert auf verschiedenen Bildschirmgrößen

## 🚀 Schnellstart

1. **Neue Präsentation erstellen:**
   ```bash
   cp template.html meine-praesentation.html
   ```

2. **Inhalt anpassen:**
   - Titel und Slides in der HTML bearbeiten
   - Logo-URL anpassen (oder entfernen)
   - Anzahl der Slides im Footer korrigieren

3. **Im Browser öffnen:**
   ```bash
   open meine-praesentation.html
   ```

## 📋 Verfügbare CSS-Klassen

### Layout-Klassen
```html
<div class="grid-2">     <!-- 2-Spalten Grid -->
<div class="grid-3">     <!-- 3-Spalten Grid -->
<div class="center">     <!-- Zentrierter Inhalt -->
```

### Spacing-Klassen
```html
<div class="mt-10">      <!-- margin-top: 10px -->
<div class="mt-20">      <!-- margin-top: 20px -->
<div class="mt-30">      <!-- margin-top: 30px -->
<div class="mt-40">      <!-- margin-top: 40px -->
<div class="mt-50">      <!-- margin-top: 50px -->
<div class="mt-5vh">     <!-- margin-top: 5vh -->
<div class="mt-10vh">    <!-- margin-top: 10vh -->
<div class="mb-20">      <!-- margin-bottom: 20px -->
<div class="mb-30">      <!-- margin-bottom: 30px -->
<div class="mb-40">      <!-- margin-bottom: 40px -->
```

### Card-Varianten
```html
<div class="card">               <!-- Standard Card (hellgrau) -->
<div class="card card-info">     <!-- Info Card (blau akzentuiert) -->
<div class="card card-neutral">  <!-- Neutrale Card (transparent grau) -->
<div class="card card-dark">     <!-- Dunkle Card (für Highlights) -->
```

### Emoji-Größen
```html
<div class="emoji">              <!-- 48px (Standard) -->
<div class="emoji-medium">       <!-- 36px -->
<div class="emoji-small">        <!-- 32px -->
```

### Phase/Kapitel-Header
```html
<div class="phase-header">
    <div class="phase-number">1</div>
    <div>
        <div class="badge">Phase 1</div>
        <h2 class="no-margin">Titel</h2>
    </div>
</div>
```

### Tech-Badges
```html
<div class="tech-stack">
    <div class="tech-badge">HTML5</div>
    <div class="tech-badge">CSS3</div>
    <div class="tech-badge">JavaScript</div>
</div>

<!-- Zentriert: -->
<div class="tech-stack center">
    <div class="tech-badge">React</div>
</div>
```

### Code-Blöcke
```html
<div class="code-block"><pre><code><span class="comment">// Kommentar</span>
<span class="keyword">const</span> variable = <span class="string">"value"</span>;
</code></pre></div>
```

## ⌨️ Navigation

- **→ / ←** - Nächste/Vorherige Slide
- **Leertaste** - Nächste Slide
- **Home** - Erste Slide
- **End** - Letzte Slide
- **Maus-Click** - Rechts = Weiter, Links = Zurück
- **Touch-Swipe** - Wischen für Navigation

## 📄 PDF-Export

So exportieren Sie die Präsentation als PDF:

### **Methode 1: Browser-Druck (Empfohlen)**

1. **Präsentation im Browser öffnen** (Chrome oder Firefox empfohlen)
2. **Drucken-Dialog öffnen:**
   - Mac: `Cmd + P`
   - Windows: `Ctrl + P`
3. **Einstellungen anpassen:**
   - **Ziel:** "Als PDF speichern"
   - **Layout:** Querformat
   - **Hintergrundgrafiken:** ✅ Aktivieren
   - **Ränder:** Keine
   - **Skalierung:** Standard (100%)
4. **Speichern klicken**

**Ergebnis:** Alle Slides werden automatisch untereinander als mehrseitiges PDF exportiert!

### **Methode 2: Screenshot-Tool (Alternative)**

Für einzelne Slides in hoher Qualität:
- Mac: `Cmd + Shift + 4` → Bereich auswählen
- Windows: Snipping Tool oder `Win + Shift + S`

### **Print-Optimierungen**

Das CSS enthält spezielle `@media print` Regeln:
- ✅ Alle Slides werden angezeigt
- ✅ Jede Slide auf eigener Seite
- ✅ Controls und Logo ausgeblendet
- ✅ Optimierte Darstellung für Druck
- ✅ Keine Page-Breaks in Code-Blöcken oder Cards

## 🎯 Slide-Typen (Beispiele in template.html)

1. **Title Slide** - Große Überschrift mit Emoji und Badge
2. **Overview** - Grid mit Cards für Themenüberblick
3. **Phase/Chapter** - Nummerierte Kapitel mit Badge
4. **List Content** - Aufzählungen mit Pfeilen
5. **Code Example** - Syntax-highlighted Code-Blöcke
6. **Two-Column** - Vergleiche oder Gegenüberstellungen
7. **Three-Column** - Mehrere gleichwertige Kategorien
8. **Tech Stack** - Badges für Technologien
9. **Mixed Content** - Kombination aus allem
10. **Summary** - Finale Zusammenfassung

## 🎨 Farben

### Hauptfarben
- **Hintergrund**: `#2c2c2e` (Apple Dark Gray)
- **Akzent**: `#0071e3` (Apple Blue)
- **Text**: `#1d1d1f` (Fast-Schwarz)

### Grau-Töne
- **Hellgrau**: `#f5f5f7` (Cards, Backgrounds)
- **Mittelgrau**: `#86868b` (Sekundärtext)
- **Dunkelgrau**: `#636366` → `#48484a` (Badges, Phase-Numbers)

### Semantische Farben
- **Info**: Blaue Akzente mit Transparenz
- **Code Keywords**: `#fc6be8` (Pink)
- **Code Strings**: `#fc8e65` (Orange)
- **Code Comments**: `#6e7681` (Grau)

## 📱 Responsive Design

- **Desktop**: Volle Größe mit Padding
- **Tablet** (< 1024px): Reduzierte Schriftgrößen, Grid → 1 Spalte
- **Mobile** (< 768px): Kompaktes Layout, kleinere Phase-Nummern

## ✨ Best Practices

1. **Konsistenz**: Nutze die vordefinierten Klassen statt Inline-Styles
2. **Hierarchie**: h1 → h2 → h3 → h4 für klare Struktur
3. **Spacing**: Nutze mt-* Klassen für einheitliche Abstände
4. **Cards**: Verwende passende Card-Varianten für verschiedene Inhalte
5. **Emojis**: Sparsam einsetzen, nur für Highlights
6. **Code**: Syntax-Highlighting mit `<span class="keyword|string|comment">`

## 🔧 Anpassungen

### Eigene Farben
Bearbeite `styles.css` und ändere die Farbwerte:
```css
.highlight { color: #0071e3; }  /* Akzentfarbe */
.badge { background: linear-gradient(...); }
```

### Eigenes Logo
Ersetze die Logo-URL in der HTML:
```html
<img src="DEINE-LOGO-URL" alt="Logo" class="logo">
```

### Schriftgrößen
Passe die Größen in `styles.css` an:
```css
h1 { font-size: 64px; }
h2 { font-size: 48px; }
```

## 📄 Lizenz

Frei verwendbar für eigene Präsentationen.

## 🙏 Credits

Inspiriert von Apple's Design-Richtlinien und modernen Web-Präsentations-Frameworks.
