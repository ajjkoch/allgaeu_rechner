# Bedienungsanleitung – Allgäu Rechner

Diese Anleitung beschreibt die Bedienung des Allgäu Rechners. Sie richtet sich an Anwenderinnen und Anwender und erläutert die Tasten, die mathematischen Funktionen sowie die Eingabe und Korrektur von Rechenausdrücken.

## 1. Überblick

Der Allgäu Rechner ist ein wissenschaftlicher Taschenrechner. Neben den vier Grundrechenarten unterstützt er unter anderem Potenzen, Wurzeln, trigonometrische Funktionen, Logarithmen, Prozentrechnung, Fakultäten und Binomialkoeffizienten.

Die Bedienung erfolgt über die Bildschirmtasten oder – sofern verfügbar – über die Tastatur. Der aktuelle Ausdruck und das Ergebnis erscheinen im Anzeigefeld.

## 2. Eine Berechnung durchführen

1. Geben Sie den Rechenausdruck über die Ziffern- und Funktionstasten ein.
2. Verwenden Sie bei Bedarf Klammern, um die gewünschte Reihenfolge festzulegen.
3. Drücken Sie **=**, um den Ausdruck auszuwerten.
4. Das Ergebnis wird im Anzeigefeld angezeigt.

Beispiel:

`(12 + 8) × 3` ergibt `60`.

Der Rechner wertet den vollständigen Ausdruck aus. Sie können deshalb auch längere Formeln eingeben, bevor Sie **=** drücken.

## 3. Zahlen und Grundrechenarten

| Taste | Funktion |
| --- | --- |
| `0` bis `9` | Ziffer eingeben |
| `.` | Dezimalpunkt eingeben |
| `+` | Addition |
| `−` | Subtraktion |
| `×` | Multiplikation |
| `/` | Division |
| `±` | Vorzeichen des Ausdrucks wechseln |
| `(` und `)` | Klammern einfügen |
| `=` | Ausdruck berechnen |
| `C` | Aktuelle Eingabe vollständig löschen |
| `⌫` | Zeichen links vom Cursor löschen |

Der Dezimaltrenner bei der Eingabe ist der Punkt.

## 4. Wissenschaftliche Funktionen

### 4.1 Potenzen

Mit der Taste **xʸ** geben Sie eine Potenz ein. Der Exponent wird nach der Taste eingegeben.

Beispiel: `2 xʸ 3` ergibt (2^3 = 8).

### 4.2 Quadratwurzel und Quadrat

Die Taste **√x** fügt die Quadratwurzelfunktion ein. Geben Sie danach den Wert und eine schließende Klammer ein.

Beispiel: `√x 25 )` ergibt (sqrt{25}=5).

Mit **INV** wird dieselbe Taste zur Quadratfunktion **x²**. Beispiel: `5 INV √x` ergibt (5^2=25).

### 4.3 n-te Wurzel

Mit **INV** wird aus **xʸ** die n-te Wurzel. Geben Sie zuerst den Radikanden, dann die Taste und anschließend den Wurzelexponenten ein.

Beispiel: `27 INV xʸ 3` ergibt (sqrt[3]{27}=3).

### 4.4 Trigonometrische Funktionen

Der Rechner bietet folgende Funktionen:

| Taste | Funktion |
| --- | --- |
| `sin` | Sinus |
| `cos` | Kosinus |
| `tan` | Tangens |

Beispiel: Im Gradmodus ergibt `sin(30)` den Wert (0{,}5).

Mit **INV** werden die Umkehrfunktionen aktiviert:

| Normalbetrieb | Mit INV |
| --- | --- |
| `sin` | `sin⁻¹` (Arkussinus) |
| `cos` | `cos⁻¹` (Arkuskosinus) |
| `tan` | `tan⁻¹` (Arkustangens) |

Die Umkehrfunktionen ermitteln aus einem Funktionswert den zugehörigen Winkel.

### 4.5 Grad und Radiant

Mit der Taste **DEG/RAD** wechseln Sie zwischen den beiden Winkelmaßen:

- **DEG**: Winkel werden in Grad interpretiert.
- **RAD**: Winkel werden im Bogenmaß (Radiant) interpretiert.

Beide Einstellungen sind insbesondere für die trigonometrischen Funktionen relevant.

Beispiele:

- Im Gradmodus: (sin(90)=1)
- Im Radiantmodus: (sin(pi/2)=1)

### 4.6 Logarithmen

| Taste | Funktion |
| --- | --- |
| `log` | Zehnerlogarithmus (Basis 10) |
| `ln` | Natürlicher Logarithmus (Basis (e)) |

Beispiele: (log(100)=2) und (ln(e)=1).

## 5. Fakultät und inverse Fakultät

### 5.1 Fakultät

Mit **n!** berechnen Sie die Fakultät einer nichtnegativen ganzen Zahl. Die Fakultät ist das Produkt aller positiven ganzen Zahlen bis einschließlich dieser Zahl.

[
5! = 5\cdot4\cdot3\cdot2\cdot1 = 120
]

Es gilt außerdem (0!=1).

### 5.2 Doppelfakultät

Die Doppelfakultät wird durch zwei Ausrufezeichen direkt im Ausdruck eingegeben: **n!!**. Dabei wird jeweils nur jede zweite Zahl multipliziert.

Beispiele:

- (5!!=5\cdot3\cdot1=15)
- (6!!=6\cdot4\cdot2=48)

Die Doppelfakultät ist eine eigene Schreibweise und bedeutet nicht, dass die Fakultät zweimal hintereinander angewendet wird.

### 5.3 Inverse Fakultät

Drücken Sie **INV** und anschließend **n!**. Der Rechner zerlegt die eingegebene positive ganze Zahl in die größtmögliche Fakultät und einen verbleibenden Rest.

Beispiele:

| Eingabe | Anzeige |
| ---: | --- |
| `5042` | `7! + 2` |
| `5402` | `7! + 362` |
| `720` | `6!` |

Ein Rest von null wird nicht zusätzlich angezeigt. Die Funktion ist für positive ganze Zahlen vorgesehen.

## 6. Prozentrechnung

Die Taste **%** berechnet einen Prozentanteil.

Beispiel: `100 % 15` ergibt `15` – 15 Prozent von 100.

Mit **INV** wird die umgekehrte Prozentrechnung aktiviert. Damit wird ermittelt, wie viel Prozent ein Wert von einem anderen Wert ausmacht.

Beispiel: `15 INV % 60` ergibt `25`, also 25 Prozent.

## 7. Weitere Funktionen und Konstanten

### 7.1 Kehrwert

Mit **1/x** wird der Kehrwert des aktuellen Ausdrucks berechnet.

Beispiel: Der Kehrwert von 4 ist (1/4=0{,}25). Der Wert null besitzt keinen Kehrwert.

### 7.2 Modulo

Die Taste **mod** berechnet den Rest einer Division.

Beispiel: `17 mod 5` ergibt `2`, denn (17=3\cdot5+2).

### 7.3 Binomialkoeffizient

Mit **C(n, k)** geben Sie einen Binomialkoeffizienten ein. Er beschreibt, auf wie viele Arten sich (k) Elemente aus (n) Elementen auswählen lassen, wenn die Reihenfolge keine Rolle spielt.

Beispiel:

[
\binom{5}{2}=10
]

### 7.4 Mathematische Konstanten

| Taste | Bedeutung |
| --- | --- |
| `π` | Kreiszahl Pi |
| `e` | Eulersche Zahl, Basis des natürlichen Logarithmus |

## 8. Die Taste INV

**INV** schaltet die alternativen Funktionen ein oder aus. Die Darstellung der betroffenen Tasten wechselt dabei zur jeweiligen Umkehr- oder Zusatzfunktion.

| Taste | Normalbetrieb | Mit INV |
| --- | --- | --- |
| `xʸ` | Potenz | n-te Wurzel |
| `√x` | Quadratwurzel | Quadrat |
| `sin` | Sinus | Arkussinus |
| `cos` | Kosinus | Arkuskosinus |
| `tan` | Tangens | Arkustangens |
| `%` | Prozentanteil | Umgekehrte Prozentrechnung |
| `n!` | Fakultät | Inverse Fakultät |

Drücken Sie **INV** erneut, um zum Normalbetrieb zurückzukehren.

## 9. Verlauf und Cursor

### 9.1 Verlauf

Mit den Verlaufstasten können Sie zuvor eingegebene Ausdrücke und Ergebnisse erneut anzeigen:

- **↶**: vorherigen Verlaufseintrag anzeigen
- **↷**: nächsten Verlaufseintrag anzeigen

### 9.2 Cursorsteuerung

Mit den Pfeiltasten bewegen Sie den Cursor innerhalb des Ausdrucks:

- Kurzes Drücken bewegt den Cursor um eine Position.
- Gedrückthalten der linken Pfeiltaste springt an den Anfang.
- Gedrückthalten der rechten Pfeiltaste springt an das Ende.

### 9.3 Eingabe korrigieren

Mit **⌫** löschen Sie das Zeichen links vom Cursor. Mit **C** löschen Sie den gesamten aktuellen Ausdruck.

## 10. Nachkommastellen einstellen

Über die Taste **fix** öffnen Sie die Einstellung für die Anzahl der angezeigten Nachkommastellen. Zulässig sind Werte von 0 bis 15.

Die Einstellung beeinflusst die Ergebnisdarstellung. Überflüssige abschließende Nullen werden nicht angezeigt. Bei sehr großen oder sehr kleinen Zahlen kann der Rechner automatisch die wissenschaftliche Schreibweise verwenden.

## 11. Fehlermeldungen und Hinweise

Ein Ausdruck kann nicht ausgewertet werden, wenn er syntaktisch fehlerhaft oder mathematisch nicht definiert ist. Typische Ursachen sind:

- Division durch null
- Fakultät einer negativen oder nicht ganzzahligen Zahl
- Quadratwurzel aus einer negativen Zahl
- Fehlende oder falsch gesetzte Klammern
- Ein mathematisch nicht definierter Funktionswert

Nach einer Fehlermeldung können Sie mit **C** eine neue Berechnung beginnen. Auch eine neue Eingabe über eine Ziffern- oder Funktionstaste beginnt nach einem Fehler mit einem neuen Ausdruck.

### Genauigkeit

Der Rechner verwendet Gleitkommazahlen. Bei bestimmten Dezimalzahlen können deshalb kleine Rundungsabweichungen auftreten. Die Einstellung der Nachkommastellen steuert die Anzeige und nicht die interne Rechengenauigkeit.

---

**Viel Freude beim Rechnen mit dem Allgäu Rechner!**
