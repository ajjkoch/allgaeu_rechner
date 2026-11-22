# Parsergrammatik des AllgäuRechners

> **Dokumenttyp:** Technische Sprach- und Parserdokumentation  
> **Implementierungsbasis:** `common/Util.h`, Klasse `Calculator`  
> **Parserverfahren:** Recursive Descent (rekursiver Abstieg)  
> **Stand:** 22. November 2026

## 1. Zweck und Geltungsbereich

Dieses Dokument beschreibt die mathematische Ausdruckssprache, die der Parser des AllgäuRechners akzeptiert. Die Grammatik ist aus der Reihenfolge und dem Verhalten der Parserfunktionen in `common/Util.h` abgeleitet. Sie soll die Implementierung dokumentieren und nicht eine davon unabhängige Wunschgrammatik definieren.

Die Grammatik beschreibt die syntaktische Struktur. Zusätzliche mathematische Bedingungen – etwa dass eine Fakultät nur für nichtnegative ganze Zahlen definiert ist – werden bei der Auswertung geprüft und sind deshalb nicht vollständig durch BNF ausdrückbar.

## 2. Die Ausdruckssprache – unabhängig von der Bedienoberfläche

Im Mittelpunkt steht nicht die Bedienung einzelner Rechnertasten, sondern das Schreiben und Auswerten eines **mathematischen Ausdrucks (expression)**. Ein Ausdruck ist eine Zeichenfolge, die Zahlen, Konstanten, Operatoren, Funktionen und Klammern kombiniert und als Ganzes einen Zahlenwert ergibt.

Beispiel:

```text
sqrt(25) mod 7 * pi
```

Die Auswertung folgt den Regeln der Operatorpräzedenz. `sqrt(25)` ergibt zunächst 5. Danach wird `5 mod 7` berechnet; das Ergebnis wird anschließend mit `pi` multipliziert. Der Ausdruck ergibt also (5\pi\), ungefähr 15,707963.

Ein Ausdruck kann damit unabhängig davon formuliert werden, ob er über die Bildschirmtasten des AllgäuRechners, über eine Tastatur oder aus einer anderen Anwendung kommt. Der Parser erhält lediglich eine Zeichenfolge und wertet sie aus.

### 2.1 Was gehört zur Syntax?

Die Syntax legt fest, **wie** ein Ausdruck geschrieben werden muss:

- Zahlen werden als Dezimalzahlen geschrieben, zum Beispiel `25`, `3.14` oder `.5`.
- Binäre Operatoren verbinden zwei Teilausdrücke, zum Beispiel `3 * 4` oder `10 mod 3`.
- Funktionen werden mit ihrem Namen und einem Argument in runden Klammern geschrieben, zum Beispiel `sqrt(25)` oder `sin(30)`.
- Klammern gruppieren Teilausdrücke, zum Beispiel `sqrt((3 + 2) * 5)`.
- Konstanten werden als `pi` und `e` geschrieben.
- Leerzeichen dürfen zur besseren Lesbarkeit verwendet werden.

Ein Ausdruck ist nicht dasselbe wie ein mathematischer Textabsatz oder ein kleines Programm. Die aktuelle Grammatik kennt beispielsweise keine Variablen, Zuweisungen, Kommentare, Funktionsdefinitionen oder mehrere durch Zeilenumbrüche getrennte Anweisungen.

### 2.2 Zusammengesetzte Ausdrücke

Die folgenden Beispiele zeigen typische Schreibweisen:

| Ausdruck | Bedeutung |
| --- | --- |
| `sqrt(25) mod 7 * pi` | Rest der Division von 5 durch 7, anschließend mal π |
| `(12 + 8) / 4` | Klammerausdruck vor der Division |
| `sin(30) + cos(60)` | Summe zweier trigonometrischer Funktionswerte |
| `2^3^2` | Potenzierung von rechts nach links |
| `5! + 3!!` | Summe aus Fakultät und Doppelfakultät |
| `10 nCr 3` | Binomialkoeffizient „10 über 3“ |
| `27 root 3` | dritte Wurzel aus 27 |
| `2pi + e` | erlaubte Kurzschreibweise für 2 · π plus e |

Die Operatoren `mod`, `nCr` und `root` werden wie andere binäre Operatoren zwischen ihre Operanden geschrieben. Die Funktionsschreibweise ist dagegen ein Präfix mit Klammerargument: erst der Funktionsname, dann die öffnende Klammer, der Ausdruck und die schließende Klammer.

### 2.3 Syntax und mathematische Bedeutung sind zwei Ebenen

Die Syntax beantwortet, ob eine Zeichenfolge strukturell ein gültiger Ausdruck ist. Die semantische Prüfung beantwortet, ob die damit verlangte Rechnung mathematisch ausgeführt werden kann.

Beispielsweise ist `5.5!` in der Form eines Ausdrucks mit Postfix-Fakultät geschrieben. Die Auswertung lehnt ihn dennoch ab, weil die Fakultät nur für nichtnegative ganze Zahlen unterstützt wird. Ebenso ist `1 / 0` syntaktisch ein korrekt aufgebauter Ausdruck, aber mathematisch nicht definiert.

### 2.4 Unabhängigkeit vom AllgäuRechner

Der eigentliche Parser ist in `common/Util.h` als C++-Klasse `Calculator` implementiert. Diese Datei verwendet Standard-C++-Bibliotheken und ist nicht von Qt, Widgets oder der grafischen Oberfläche abhängig. Die Grammatik kann deshalb grundsätzlich auch in einem anderen C++-Programm verwendet werden, das diese Klasse einbindet.

Die **Ausdruckssyntax ist weitgehend eine eigenständige mathematische Sprache**. Einige Auswertungsdetails sind allerdings konfigurierbar bzw. durch den Rechner geprägt:

- Trigonometrische Funktionen verwenden je nach Einstellung Grad oder Radiant.
- Der INV-Zustand verändert die Bedeutung des Prozentoperators.
- Die Funktions- und Operatornamen entsprechen dem derzeitigen Funktionsumfang dieses Parsers.

Der Parser selbst enthält keine Dateiverarbeitung. Ein anderes Programm könnte aber beispielsweise eine Textdatei zeilenweise einlesen und jede nichtleere Zeile als einzelnen Ausdruck an `Calculator::evaluate()` übergeben. Damit ließen sich Rechenlisten oder einfache Ausdruckssammlungen aus Textdateien auswerten, ohne die Qt-Oberfläche zu verwenden.

Dafür müsste die aufrufende Anwendung die Datei öffnen, Zeilen trennen, Kommentare oder Leerzeilen – falls gewünscht – selbst behandeln und die Ergebnisse ausgeben. Die aktuelle Grammatik definiert weder ein Dateiformat noch eine Sprache mit Variablen oder mehreren Anweisungen. Eine solche dateibasierte Auswertung wäre also eine Nutzung des vorhandenen Ausdrucksparsers durch ein separates Programm und noch keine eingebaute Funktion des AllgäuRechners.

## 3. Notation

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
