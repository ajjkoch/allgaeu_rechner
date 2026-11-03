# Calculator test overview

This document describes the automated tests for the Qt calculator.

## Test execution

The GitHub Actions workflow `.github/workflows/calculator.yml` builds the
calculator and runs two independent Qt Test executables:

- `CalculatorGuiTests`: GUI interaction tests from
  `UnitTests/Test_MainWindow.cpp`, executed with
  `QT_QPA_PLATFORM=offscreen`.
- `Test_Calculator`: parser and calculation tests from
  `UnitTests/Test_Calculator.cpp`.

Run both locally from the repository root:

```sh
cmake -S instructor/Calculator_cmake -B build/calculator -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/calculator --parallel
ctest --test-dir build/calculator --output-on-failure -R CalculatorGuiTests

cmake -S instructor/Calculator_cmake/UnitTests -B build/calculator-tests -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/calculator-tests --parallel
ctest --test-dir build/calculator-tests --output-on-failure
```

## Current inventory

The current suite contains **70 test methods**:

| Test layer | Test methods | Main focus |
|---|---:|---|
| Parser/calculation unit tests | 31 | Arithmetic, parsing, operators, functions, numeric boundaries and exceptions |
| GUI interaction tests | 39 | Button interaction, display editing, keyboard input, modes, visible results and error recovery |
| **Total** | **70** | |

Qt's `init()` setup method is not included in the method count.

## Coverage by topic

### Parser and calculation layer

- Basic arithmetic and operator precedence, including unary signs and right-associative powers
- Parentheses, nested expressions, whitespace and malformed input
- Decimal and scientific-notation literals, pi and the supported implicit `2pi` shorthand
- Division, modulo and percentage operations, including inverted percentage mode
- Factorial boundaries and invalid operands
- Square root, logarithms, exponential and reciprocal functions
- Trigonometric functions in degree and radian modes; inverse trigonometric domains
- Overflow, underflow, subnormal values and very small non-zero operands
- Parser recursion-depth limits
- Independence of separate calculator instances

### GUI layer

- Digit and operator buttons, parentheses, percentages and constants
- Editing the display, cursor/backspace behavior and history navigation
- Keyboard entry, Backspace, decimal point, parentheses, sign and Enter evaluation
- Trigonometric functions, inverse functions and switching between degree/radian modes
- Error display and recovery after invalid calculations
- Large-number, exponentiation and factorial boundary scenarios

## Known limitations and follow-up

- The suite is broad, but it does not establish formal correctness for every possible expression.
- The workflow does not currently collect line, branch or mutation coverage. Therefore no percentage coverage claim is made.
- GUI tests run headlessly on Linux/Qt 6. They do not replace manual checks of visual layout, accessibility, focus behavior on every desktop platform, or native keyboard behavior on Windows/macOS.
- Some legacy GUI tests only check that an error replaced the expression. The principal error cases now assert the exact diagnostic, but the older cases can be tightened incrementally.
- A few parser behaviors deserve explicit future tests: repeated factorial (for example `3!!`), factorial precedence/sign combinations, malformed scientific notation in the GUI, and parser nesting limits through the GUI. Parser recursion limits are already tested at the unit-test layer.
- Floating-point assertions should use tolerances for transcendental results; exact equality is appropriate only for exactly representable or deliberately formatted results.

## Maintenance rule

When adding a calculator feature or changing parsing/formatting behavior, add or update:
1. a parser-level test for the calculation semantics and exceptional cases; and
2. a GUI-level test when the behavior is exposed through a button, display editing or keyboard interaction.

Keep the GUI and parser tests independent: a passing parser test does not prove that the GUI wires the feature correctly.
