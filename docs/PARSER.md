# Parsergrammatik des AllgäuRechners

> **Dokumenttyp:** Technische Sprach- und Parserdokumentation  
> **Implementierungsbasis:** `common/Util.h`, Klasse `Calculator`  
> **Parserverfahren:** Recursive Descent (rekursiver Abstieg)  
> **Stand:** 22. November 2026

## 1. Zweck und Geltungsbereich

Dieses Dokument beschreibt die mathematische Ausdruckssprache, die der Parser des AllgäuRechners akzeptiert. Die Grammatik ist aus der Reihenfolge und dem Verhalten der Parserfunktionen in `common/Util.h` abgeleitet. Sie soll die Implementierung dokumentieren und nicht eine davon unabhängige Wunschgrammatik definieren.

Die Grammatik beschreibt die syntaktische Struktur. Zusätzliche mathematische Bedingungen – etwa dass eine Fakultät nur für nichtnegative ganze Zahlen definiert ist – werden bei der Auswertung geprüft und sind deshalb nicht vollständig durch BNF ausdrückbar.

## 2. Notation

Die Grammatik verwendet eine lesbare BNF-Notation:

- `<Nichtterminal>` bezeichnet eine syntaktische Regel.
- Terminale stehen in Anführungszeichen, beispielsweise `"+"`.
- `|` bedeutet Alternative.
- `ε` bezeichnet die leere Zeichenfolge.
- Wiederholungen und optionale Teile werden in den Regeln durch rekursive Definitionen ausgedrückt.

Die Schreibweise ist bewusst kompakt. Die lexikalischen Regeln für Zahlen und Buchstaben werden ergänzend beschrieben, da die Implementierung dafür C/C++-Funktionen wie `std::strtod`, `std::isalpha` und `std::isspace` verwendet.

## 3. Grammatik in BNF

### 3.1 Vollständiger Ausdruck und Operatorpräzedenz

Die Reihenfolge der Regeln bildet die Präzedenz von niedrig nach hoch ab:

```bnf
<expression>       ::= <addition>

<addition>         ::= <multiplication>
                     | <addition> "+" <multiplication>
                     | <addition> "-" <multiplication>

<multiplication>   ::= <unary>
                     | <multiplication> "*" <unary>
                     | <multiplication> "/" <unary>
                     | <multiplication> "%" <unary>
                     | <multiplication> "mod" <unary>
                     | <multiplication> "nCr" <unary>
                     | <multiplication> "root" <unary>

<unary>            ::= <power>
                     | "+" <unary>
                     | "-" <unary>

<power>            ::= <postfix>
                     | <postfix> "^" <unary>

<postfix>          ::= <primary>
                     | <postfix> "!"
                     | <postfix> "!!"

<primary>          ::= <number>
                     | <number> "pi"
                     | "pi"
                     | "e"
                     | "(" <addition> ")"
                     | <function> "(" <addition> ")"

<function>         ::= "sqrt"
                     | "sin"
                     | "cos"
                     | "tan"
                     | "asin"
                     | "acos"
                     | "atan"
                     | "log"
                     | "ln"
                     | "exp"
                     | "edx"
```

Die Regel `<postfix>` lässt wiederholte Postfix-Operatoren zu. Bei jedem Fakultätszeichen prüft der Parser, ob unmittelbar ein zweites `!` folgt. Daher ist `!!` ein eigener Operator und nicht die zweimalige Anwendung der einfachen Fakultät.

### 3.2 Zahlen und lexikalische Elemente

Die Zahlenerkennung erfolgt mit `std::strtod`. Damit werden Dezimalzahlen einschließlich optionalem Exponenten erkannt. Die folgende Regel beschreibt die übliche dezimale Schreibweise, die in der Rechneroberfläche verwendet wird:

```bnf
<number>           ::= <digits>
                     | <digits> "." <digits>
                     | "." <digits>
                     | <digits> <exponent>
                     | <digits> "." <digits> <exponent>
                     | "." <digits> <exponent>

<exponent>         ::= ("e" | "E") ["+" | "-"] <digits>

<digits>           ::= <digit>
                     | <digits> <digit>

<digit>            ::= "0" | "1" | "2" | "3" | "4"
                     | "5" | "6" | "7" | "8" | "9"
```

Leerraum (Whitespace) wird an den Stellen übersprungen, an denen der Parser ein Zeichen oder Schlüsselwort konsumiert. Eine Besonderheit ist die Kurzschreibweise für die Kreiszahl: Eine Zahl kann direkt und ohne Zwischenraum mit `pi` verbunden werden, beispielsweise `2pi`. Eine allgemeine implizite Multiplikation – etwa `2(3+4)` oder `2sin(30)` – ist nicht implementiert.

Die Schlüsselwörter `mod`, `nCr` und `root` müssen als vollständiges Wort erkannt werden. Auf ein Schlüsselwort darf nicht unmittelbar ein weiterer alphabetischer Buchstabe folgen. Funktionsnamen werden als zusammenhängende Buchstabenfolge gelesen und müssen anschließend von einer öffnenden Klammer gefolgt sein.

## 4. Präzedenz und Assoziativität

| Ebene | Konstrukte | Assoziativität / Verhalten |
| --- | --- | --- |
| 1 – niedrig | `+`, `-` | linksassoziativ |
| 2 | `*`, `/`, `%`, `mod`, `nCr`, `root` | linksassoziativ und gleiche Präzedenz |
| 3 | unäres `+`, unäres `-` | rekursiv, bindet schwächer als Potenz |
| 4 | `^` | rechtsassoziativ |
| 5 – hoch | `!`, `!!` | Postfix, mehrfach anwendbar |
| 6 | Zahlen, Konstanten, Klammern, Funktionen | Primärausdrücke |

Daraus folgen beispielsweise:

| Ausdruck | Interpretation |
| --- | --- |
| `2 + 3 * 4` | `2 + (3 * 4)` |
| `2^3^2` | `2^(3^2)` |
| `-2^2` | `-(2^2)` |
| `2^-3` | `2^(-3)` |
| `5!!` | Doppelfakultät von 5 |
| `3 + 4 * 2!` | `3 + (4 * (2!))` |

Insbesondere haben `nCr` und `root` dieselbe Präzedenz wie Multiplikation und Division. Die Auswertung erfolgt bei Ketten von Operatoren dieser Ebene von links nach rechts.

## 5. Operatoren und ihre Bedeutung

| Terminal | Bedeutung |
| --- | --- |
| `+`, `-` | Addition und Subtraktion; außerdem unäre Vorzeichen |
| `*`, `/` | Multiplikation und Division |
| `%` | Prozent: `a % b` berechnet standardmäßig `a * b / 100` |
| `mod` | Gleitkomma-Rest gemäß `std::fmod` |
| `^` | Potenzierung |
| `root` | Wurzeloperator: `y root x` bedeutet die x-te Wurzel aus y |
| `!` | Fakultät |
| `!!` | Doppelfakultät |
| `nCr` | Binomialkoeffizient „n über k“ |

Der INV-Zustand verändert die Bedeutung des Prozentoperators. Im invertierten Modus berechnet `a % b` den Wert `(a / b) * 100`. Diese Zustandsabhängigkeit ist semantisch und verändert die Grammatik nicht.

## 6. Funktionen und Konstanten

Alle Funktionen erwarten genau ein Argument in runden Klammern:

| Funktion | Bedeutung |
| --- | --- |
| `sqrt(x)` | Quadratwurzel |
| `sin(x)`, `cos(x)`, `tan(x)` | trigonometrische Funktionen |
| `asin(x)`, `acos(x)`, `atan(x)` | inverse trigonometrische Funktionen |
| `log(x)` | dekadischer Logarithmus |
| `ln(x)` | natürlicher Logarithmus |
| `exp(x)` | Exponentialfunktion (e^x) |
| `edx(x)` | Kehrwert (1/x) |

Die Konstanten sind `pi` und `e`. Trigonometrische Funktionen arbeiten standardmäßig mit Grad; über den Winkelmodus kann auf Radiant umgeschaltet werden. Die inverse Winkelfunktion liefert entsprechend dem aktiven Modus Grad oder Radiant.

## 7. Syntaktische und semantische Gültigkeit

Ein Ausdruck kann syntaktisch korrekt sein und dennoch bei der mathematischen Auswertung scheitern.

Beispiele für semantische Bedingungen:

- `!` und `!!` verlangen einen nichtnegativen ganzzahligen Operanden.
- `nCr` verlangt nichtnegative ganze Zahlen mit (k \le n).
- Division und Modulo durch null sind nicht zulässig.
- `sqrt(x)` verlangt (x \ge 0).
- `ln(x)` und `log(x)` verlangen (x > 0).
- `asin(x)` und `acos(x)` verlangen (-1 \le x \le 1).
- `edx(x)` verlangt (x \ne 0).
- Eine gerade Wurzel aus einer negativen Zahl ist nicht definiert.
- Ein Ausdruck darf die maximale Parser-Verschachtelungstiefe von 256 nicht überschreiten.

Nach dem Parsen des Ausdrucks prüft der Parser, ob die gesamte Eingabe verbraucht wurde. Zusätzliche oder ungültige Zeichen am Ende führen daher zu einem Fehler.

## 8. Beispiele

| Ausdruck | Ergebnis / Bewertung |
| --- | --- |
| `2 + 3 * 4` | gültig, Ergebnis 14 |
| `2^3^2` | gültig, Ergebnis 512 |
| `-2^2` | gültig, Ergebnis -4 |
| `2^-3` | gültig, Ergebnis 0,125 |
| `5!!` | gültig, Ergebnis 15 |
| `6!!` | gültig, Ergebnis 48 |
| `sqrt(81) + log(100)` | gültig, Ergebnis 11 |
| `2pi` | gültige Kurzschreibweise für (2\pi) |
| `2(3+4)` | ungültig: implizite Multiplikation fehlt |
| `sin 30` | ungültig: Funktionsklammern fehlen |
| `5.5!` | syntaktisch erkennbar, aber semantisch ungültig |
| `1/0` | syntaktisch korrekt, aber Division durch null |

## 9. Zuordnung zu den Parserfunktionen

Die BNF-Regeln entsprechen unmittelbar den Methoden der inneren Klasse `Calculator::Parser`:

| BNF-Regel | Implementierung |
| --- | --- |
| `<expression>` | `parse()` |
| `<addition>` | `parseAdditionSubtraction()` |
| `<multiplication>` | `parseMultiplicationDivision()` |
| `<unary>` | `parseUnary()` |
| `<power>` | `parsePower()` |
| `<postfix>` | `parsePostfix()` |
| `<primary>` | `parsePrimary()` |
| `<function>` | `parseFunction()` |

Die Grammatik sollte bei Änderungen an diesen Methoden mit angepasst werden. Insbesondere Änderungen an der Reihenfolge der Parse-Aufrufe können die Präzedenz oder Assoziativität der Operatoren verändern.

## 10. Implementierungsquelle

Die maßgebliche Implementierung befindet sich in [`common/Util.h`](../common/Util.h) auf dem Branch `double_factorial`. Die technische Gesamtarchitektur ist in [`DESIGN.md`](DESIGN.md) beschrieben.
