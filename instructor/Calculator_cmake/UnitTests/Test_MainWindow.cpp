#include <QtTest>

#include <QPushButton>
#include <QTextEdit>

#include "mainwindow.h"

class CalculatorGuiTests : public QObject
{
    Q_OBJECT

private slots:
    void digitButtonsAndAddition();
    void cursorEditingAndBackspace();
    void directDisplayEditing();
    void errorRecovery();
    void historyNavigation();
    void signToggle();
    void parenthesesAndMultiplication();
    void percentageOperation();
    void trigonometricFunctions();
    void squareRootAndPower();
    void tangentInDegrees();
    void radiansMode();
    void logarithmFunctions();
    void reciprocalOperation();
    void mathematicalFunctionErrors();
    void cosineInDegrees();
    void moduloOperation();
    void inverseTangentInDegrees();
    void tangentDomainError();
    void exponentialAndFactorial();
    void piConstantAndDecimalInput();
    void inversePercentageMode();
    void inverseTrigonometricDomainError();
    void reciprocalOfZeroError();
    void keyboardEntry();
    void divisionByZeroError();
    void moduloByZeroError();
    void inverseCosineDomainError();
    void logarithmDomainErrors();
    void tangentDomainErrorInRadians();
    void keyboardBackspace();
    void keyboardDecimalAndParentheses();
    void keyboardNegativeSign();
    void keyboardEnterEvaluates();
    void angleModeCanToggleBackToDegrees();
    void inverseTrigonometricFunctionsInRadians();
    void negativeExponentAndNestedExpressions();
    void factorialBoundaryErrors();
    void largeNumberOverflowErrors();
    void repeatedFactorialAndSignPrecedence();
    void malformedScientificNotationInDisplay();
    void parserNestingLimitInDisplay();
    void guiBoundaryBranches();
    void guiSignAndEventFilterBoundaries();
};

void CalculatorGuiTests::digitButtonsAndAddition()
{
    MainWindow window;
    window.show();

    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *plus = window.findChild<QPushButton *>("pushButton_plus");
    auto *three = window.findChild<QPushButton *>("pushButton_3");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *display = window.findChild<QTextEdit *>("textEdit");

    QVERIFY(two);
    QVERIFY(plus);
    QVERIFY(three);
    QVERIFY(equals);
    QVERIFY(display);

    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(plus, Qt::LeftButton);
    QTest::mouseClick(three, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("2+3"));

    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("5"));
}

void CalculatorGuiTests::cursorEditingAndBackspace()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *three = window.findChild<QPushButton *>("pushButton_3");
    auto *left = window.findChild<QPushButton *>("pushButton_cursor_left");
    auto *backspace = window.findChild<QPushButton *>("pushButton_erase_left");

    QVERIFY(display);
    QVERIFY(one);
    QVERIFY(two);
    QVERIFY(three);
    QVERIFY(left);
    QVERIFY(backspace);

    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(three, Qt::LeftButton);
    QTest::mouseClick(left, Qt::LeftButton);
    QTest::mouseClick(window.findChild<QPushButton *>("pushButton_9"), Qt::LeftButton);

    QCOMPARE(display->toPlainText(), QString("1293"));

    QTest::mouseClick(backspace, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("123"));
}

void CalculatorGuiTests::directDisplayEditing()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    QVERIFY(display);
    QVERIFY(equals);

    display->setPlainText("6*7");
    QTest::mouseClick(equals, Qt::LeftButton);

    QCOMPARE(display->toPlainText(), QString("42"));
}

void CalculatorGuiTests::errorRecovery()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *divide = window.findChild<QPushButton *>("pushButton_divided");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *seven = window.findChild<QPushButton *>("pushButton_7");

    QVERIFY(display);
    QVERIFY(one);
    QVERIFY(divide);
    QVERIFY(zero);
    QVERIFY(equals);
    QVERIFY(seven);

    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(divide, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("Division by zero"));

    QTest::mouseClick(seven, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("7"));
}

void CalculatorGuiTests::historyNavigation()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *plus = window.findChild<QPushButton *>("pushButton_plus");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *historyBack = window.findChild<QPushButton *>("pushButton_history_back");
    auto *historyForward = window.findChild<QPushButton *>("pushButton_history_forward");

    QVERIFY(display);
    QVERIFY(two);
    QVERIFY(plus);
    QVERIFY(equals);
    QVERIFY(historyBack);
    QVERIFY(historyForward);

    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(plus, Qt::LeftButton);
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("4"));

    QTest::mouseClick(historyBack, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("2+2"));

    QTest::mouseClick(historyForward, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("4"));
}

void CalculatorGuiTests::signToggle()
{
    MainWindow window;
    window.show();

    auto *five = window.findChild<QPushButton *>("pushButton_5");
    auto *sign = window.findChild<QPushButton *>("pushButton_plusminus");
    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(five);
    QVERIFY(sign);
    QVERIFY(display);
    QVERIFY(equals);

    QTest::mouseClick(five, Qt::LeftButton);
    QTest::mouseClick(sign, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("-5"));

    QTest::mouseClick(sign, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("5"));

    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("5"));
}

void CalculatorGuiTests::parenthesesAndMultiplication()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *left = window.findChild<QPushButton *>("pushButton_bracket_left");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *plus = window.findChild<QPushButton *>("pushButton_plus");
    auto *three = window.findChild<QPushButton *>("pushButton_3");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *multiply = window.findChild<QPushButton *>("pushButton_mul");
    auto *four = window.findChild<QPushButton *>("pushButton_4");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(left);
    QVERIFY(two);
    QVERIFY(plus);
    QVERIFY(three);
    QVERIFY(right);
    QVERIFY(multiply);
    QVERIFY(four);
    QVERIFY(equals);

    QTest::mouseClick(left, Qt::LeftButton);
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(plus, Qt::LeftButton);
    QTest::mouseClick(three, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QTest::mouseClick(multiply, Qt::LeftButton);
    QTest::mouseClick(four, Qt::LeftButton);

    QCOMPARE(display->toPlainText(), QString("(2+3)*4"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("20"));
}

void CalculatorGuiTests::percentageOperation()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *five = window.findChild<QPushButton *>("pushButton_5");
    auto *percent = window.findChild<QPushButton *>("pushButton_percent");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(one);
    QVERIFY(zero);
    QVERIFY(five);
    QVERIFY(percent);
    QVERIFY(equals);

    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(percent, Qt::LeftButton);
    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(five, Qt::LeftButton);

    QCOMPARE(display->toPlainText(), QString("100%15"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("15"));
}

void CalculatorGuiTests::trigonometricFunctions()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *sin = window.findChild<QPushButton *>("pushButton_sin");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *nine = window.findChild<QPushButton *>("pushButton_9");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *inverse = window.findChild<QPushButton *>("pushButton_inv");

    QVERIFY(display);
    QVERIFY(sin);
    QVERIFY(one);
    QVERIFY(zero);
    QVERIFY(nine);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(inverse);

    // The default angle mode is degrees: sin(90) = 1.
    QTest::mouseClick(sin, Qt::LeftButton);
    QTest::mouseClick(nine, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("sin(90)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("1"));

    // Start a fresh expression before testing inverse sine.
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");
    QVERIFY(clear);
    QTest::mouseClick(clear, Qt::LeftButton);

    // Inverse sine is also expressed in degrees by default: asin(1) = 90.
    QTest::mouseClick(inverse, Qt::LeftButton);
    QTest::mouseClick(sin, Qt::LeftButton);
    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("asin(1)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("90"));
}

void CalculatorGuiTests::squareRootAndPower()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *sqrt = window.findChild<QPushButton *>("pushButton_sqrt");
    auto *power = window.findChild<QPushButton *>("pushButton_x2");
    auto *three = window.findChild<QPushButton *>("pushButton_3");
    auto *nine = window.findChild<QPushButton *>("pushButton_9");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");

    QVERIFY(display);
    QVERIFY(sqrt);
    QVERIFY(power);
    QVERIFY(three);
    QVERIFY(nine);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(clear);

    QTest::mouseClick(sqrt, Qt::LeftButton);
    QTest::mouseClick(nine, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("sqrt(9)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("3"));

    QTest::mouseClick(clear, Qt::LeftButton);
    QTest::mouseClick(three, Qt::LeftButton);
    QTest::mouseClick(power, Qt::LeftButton);
    QTest::mouseClick(three, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("3^3"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("27"));
}

void CalculatorGuiTests::tangentInDegrees()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *tan = window.findChild<QPushButton *>("pushButton_tan");
    auto *four = window.findChild<QPushButton *>("pushButton_4");
    auto *five = window.findChild<QPushButton *>("pushButton_5");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(tan);
    QVERIFY(four);
    QVERIFY(five);
    QVERIFY(right);
    QVERIFY(equals);

    // The initial angle mode is degrees, so tan(45) is approximately 1.
    QTest::mouseClick(tan, Qt::LeftButton);
    QTest::mouseClick(four, Qt::LeftButton);
    QTest::mouseClick(five, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("tan(45)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - 1.0) < 1e-12);
}

void CalculatorGuiTests::radiansMode()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *mode = window.findChild<QPushButton *>("pushButton_changeDegreesMode");
    auto *sin = window.findChild<QPushButton *>("pushButton_sin");
    auto *pi = window.findChild<QPushButton *>("pushButton_pi");
    auto *divide = window.findChild<QPushButton *>("pushButton_divided");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(mode);
    QVERIFY(sin);
    QVERIFY(pi);
    QVERIFY(divide);
    QVERIFY(two);
    QVERIFY(right);
    QVERIFY(equals);

    QCOMPARE(mode->text(), QString("grad"));
    QTest::mouseClick(mode, Qt::LeftButton);
    QCOMPARE(mode->text(), QString("rad"));

    // In radians, sin(pi/2) is exactly 1 within floating-point precision.
    QTest::mouseClick(sin, Qt::LeftButton);
    QTest::mouseClick(pi, Qt::LeftButton);
    QTest::mouseClick(divide, Qt::LeftButton);
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("sin(pi/2)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - 1.0) < 1e-12);
}


void CalculatorGuiTests::logarithmFunctions()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *log = window.findChild<QPushButton *>("pushButton_log");
    auto *ln = window.findChild<QPushButton *>("pushButton_ln");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");

    QVERIFY(display);
    QVERIFY(log);
    QVERIFY(ln);
    QVERIFY(one);
    QVERIFY(zero);
    QVERIFY(two);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(clear);

    QTest::mouseClick(log, Qt::LeftButton);
    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("log(100)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("2"));

    QTest::mouseClick(clear, Qt::LeftButton);
    QTest::mouseClick(ln, Qt::LeftButton);
    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("ln(1)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("0"));
}

void CalculatorGuiTests::reciprocalOperation()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *four = window.findChild<QPushButton *>("pushButton_4");
    auto *reciprocal = window.findChild<QPushButton *>("pushButton_edx");

    QVERIFY(display);
    QVERIFY(four);
    QVERIFY(reciprocal);

    QTest::mouseClick(four, Qt::LeftButton);
    QTest::mouseClick(reciprocal, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - 0.25) < 1e-12);
}

void CalculatorGuiTests::mathematicalFunctionErrors()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *sqrt = window.findChild<QPushButton *>("pushButton_sqrt");
    auto *minus = window.findChild<QPushButton *>("pushButton_minus");
    auto *nine = window.findChild<QPushButton *>("pushButton_9");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");
    auto *seven = window.findChild<QPushButton *>("pushButton_7");

    QVERIFY(display);
    QVERIFY(sqrt);
    QVERIFY(minus);
    QVERIFY(nine);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(clear);
    QVERIFY(seven);

    QTest::mouseClick(sqrt, Qt::LeftButton);
    QTest::mouseClick(minus, Qt::LeftButton);
    QTest::mouseClick(nine, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("sqrt(-9)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("sqrt requires a non-negative argument"));

    QTest::mouseClick(seven, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("7"));

}


void CalculatorGuiTests::cosineInDegrees()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *cos = window.findChild<QPushButton *>("pushButton_cos");
    auto *six = window.findChild<QPushButton *>("pushButton_6");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(cos);
    QVERIFY(six);
    QVERIFY(zero);
    QVERIFY(right);
    QVERIFY(equals);

    QTest::mouseClick(cos, Qt::LeftButton);
    QTest::mouseClick(six, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("cos(60)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - 0.5) < 1e-12);
}

void CalculatorGuiTests::moduloOperation()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *three = window.findChild<QPushButton *>("pushButton_3");
    auto *mod = window.findChild<QPushButton *>("pushButton_mod");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(one);
    QVERIFY(zero);
    QVERIFY(three);
    QVERIFY(mod);
    QVERIFY(equals);

    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(mod, Qt::LeftButton);
    QTest::mouseClick(three, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("10 mod 3"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("1"));
}

void CalculatorGuiTests::inverseTangentInDegrees()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *inverse = window.findChild<QPushButton *>("pushButton_inv");
    auto *tan = window.findChild<QPushButton *>("pushButton_tan");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(inverse);
    QVERIFY(tan);
    QVERIFY(one);
    QVERIFY(right);
    QVERIFY(equals);

    QTest::mouseClick(inverse, Qt::LeftButton);
    QCOMPARE(tan->text(), QString("atan"));
    QTest::mouseClick(tan, Qt::LeftButton);
    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("atan(1)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("45"));
}

void CalculatorGuiTests::tangentDomainError()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *tan = window.findChild<QPushButton *>("pushButton_tan");
    auto *nine = window.findChild<QPushButton *>("pushButton_9");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");

    QVERIFY(display);
    QVERIFY(tan);
    QVERIFY(nine);
    QVERIFY(zero);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(clear);

    QTest::mouseClick(tan, Qt::LeftButton);
    QTest::mouseClick(nine, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("tan(90)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("tan is undefined at odd multiples of 90 degrees"));

    QTest::mouseClick(clear, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString());
}


void CalculatorGuiTests::exponentialAndFactorial()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");

    QVERIFY(display);
    QVERIFY(equals);
    QVERIFY(clear);

    display->setPlainText("exp(1)");
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - qExp(1.0)) < 1e-5);

    QTest::mouseClick(clear, Qt::LeftButton);
    display->setPlainText("5!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("120"));
}

void CalculatorGuiTests::piConstantAndDecimalInput()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *pi = window.findChild<QPushButton *>("pushButton_pi");
    auto *decimal = window.findChild<QPushButton *>("pushButton_decimal");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *five = window.findChild<QPushButton *>("pushButton_5");
    auto *plus = window.findChild<QPushButton *>("pushButton_plus");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");

    QVERIFY(display);
    QVERIFY(pi);
    QVERIFY(decimal);
    QVERIFY(one);
    QVERIFY(five);
    QVERIFY(plus);
    QVERIFY(zero);
    QVERIFY(equals);
    QVERIFY(clear);

    QTest::mouseClick(pi, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - M_PI) < 1e-5);

    QTest::mouseClick(clear, Qt::LeftButton);
    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(decimal, Qt::LeftButton);
    QTest::mouseClick(five, Qt::LeftButton);
    QTest::mouseClick(plus, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(decimal, Qt::LeftButton);
    QTest::mouseClick(five, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("1.5+0.5"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("2"));
}

void CalculatorGuiTests::inversePercentageMode()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *inverse = window.findChild<QPushButton *>("pushButton_inv");
    auto *percent = window.findChild<QPushButton *>("pushButton_percent");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *five = window.findChild<QPushButton *>("pushButton_5");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");

    QVERIFY(display);
    QVERIFY(inverse);
    QVERIFY(percent);
    QVERIFY(two);
    QVERIFY(zero);
    QVERIFY(five);
    QVERIFY(equals);
    QVERIFY(clear);

    QTest::mouseClick(inverse, Qt::LeftButton);
    QCOMPARE(percent->text(), QString("1/%"));
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(percent, Qt::LeftButton);
    QTest::mouseClick(five, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("200%50"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("400"));

    QTest::mouseClick(clear, Qt::LeftButton);
    QTest::mouseClick(inverse, Qt::LeftButton);
    QCOMPARE(percent->text(), QString("%"));
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(percent, Qt::LeftButton);
    QTest::mouseClick(five, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("100"));
}

void CalculatorGuiTests::inverseTrigonometricDomainError()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *inverse = window.findChild<QPushButton *>("pushButton_inv");
    auto *sin = window.findChild<QPushButton *>("pushButton_sin");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(inverse);
    QVERIFY(sin);
    QVERIFY(two);
    QVERIFY(right);
    QVERIFY(equals);

    QTest::mouseClick(inverse, Qt::LeftButton);
    QTest::mouseClick(sin, Qt::LeftButton);
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("asin(2)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("asin argument must be between -1 and 1"));
}

void CalculatorGuiTests::reciprocalOfZeroError()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *reciprocal = window.findChild<QPushButton *>("pushButton_edx");

    QVERIFY(display);
    QVERIFY(zero);
    QVERIFY(reciprocal);

    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(reciprocal, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("Reciprocal of zero"));
}

void CalculatorGuiTests::keyboardEntry()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(equals);

    display->setFocus();
    QTest::keyClicks(display, "12+3");
    QCOMPARE(display->toPlainText(), QString("12+3"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("15"));
}


void CalculatorGuiTests::divisionByZeroError()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *divide = window.findChild<QPushButton *>("pushButton_divided");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *seven = window.findChild<QPushButton *>("pushButton_7");

    QVERIFY(display);
    QVERIFY(one);
    QVERIFY(divide);
    QVERIFY(zero);
    QVERIFY(equals);
    QVERIFY(seven);

    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(divide, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);

    QCOMPARE(display->toPlainText(), QString("Division by zero"));

    QTest::mouseClick(seven, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("7"));
}

void CalculatorGuiTests::moduloByZeroError()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *mod = window.findChild<QPushButton *>("pushButton_mod");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *seven = window.findChild<QPushButton *>("pushButton_7");

    QVERIFY(display);
    QVERIFY(one);
    QVERIFY(zero);
    QVERIFY(mod);
    QVERIFY(equals);
    QVERIFY(seven);

    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(mod, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);

    QCOMPARE(display->toPlainText(), QString("Modulo by zero"));

    QTest::mouseClick(seven, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("7"));
}

void CalculatorGuiTests::inverseCosineDomainError()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *inverse = window.findChild<QPushButton *>("pushButton_inv");
    auto *cos = window.findChild<QPushButton *>("pushButton_cos");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *seven = window.findChild<QPushButton *>("pushButton_7");

    QVERIFY(display);
    QVERIFY(inverse);
    QVERIFY(cos);
    QVERIFY(two);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(seven);

    QTest::mouseClick(inverse, Qt::LeftButton);
    QTest::mouseClick(cos, Qt::LeftButton);
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("acos(2)"));
    QTest::mouseClick(equals, Qt::LeftButton);

    QCOMPARE(display->toPlainText(),
             QString("acos argument must be between -1 and 1"));

    QTest::mouseClick(seven, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("7"));
}

void CalculatorGuiTests::logarithmDomainErrors()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *log = window.findChild<QPushButton *>("pushButton_log");
    auto *ln = window.findChild<QPushButton *>("pushButton_ln");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *minus = window.findChild<QPushButton *>("pushButton_minus");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");

    QVERIFY(display);
    QVERIFY(log);
    QVERIFY(ln);
    QVERIFY(zero);
    QVERIFY(one);
    QVERIFY(minus);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(clear);

    QTest::mouseClick(log, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("log(0)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("log requires a positive argument"));

    QTest::mouseClick(clear, Qt::LeftButton);
    QTest::mouseClick(ln, Qt::LeftButton);
    QTest::mouseClick(minus, Qt::LeftButton);
    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("ln(-1)"));
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("ln requires a positive argument"));

    QTest::mouseClick(zero, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("0"));
}

void CalculatorGuiTests::tangentDomainErrorInRadians()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *mode = window.findChild<QPushButton *>("pushButton_changeDegreesMode");
    auto *tan = window.findChild<QPushButton *>("pushButton_tan");
    auto *pi = window.findChild<QPushButton *>("pushButton_pi");
    auto *divide = window.findChild<QPushButton *>("pushButton_divided");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *seven = window.findChild<QPushButton *>("pushButton_7");

    QVERIFY(display);
    QVERIFY(mode);
    QVERIFY(tan);
    QVERIFY(pi);
    QVERIFY(divide);
    QVERIFY(two);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(seven);

    QTest::mouseClick(mode, Qt::LeftButton);
    QCOMPARE(mode->text(), QString("rad"));

    QTest::mouseClick(tan, Qt::LeftButton);
    QTest::mouseClick(pi, Qt::LeftButton);
    QTest::mouseClick(divide, Qt::LeftButton);
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("tan(pi/2)"));
    QTest::mouseClick(equals, Qt::LeftButton);

    QCOMPARE(display->toPlainText(),
             QString("tan is undefined at odd multiples of 90 degrees"));

    QTest::mouseClick(seven, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("7"));
}


void CalculatorGuiTests::keyboardBackspace()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    QVERIFY(display);

    display->setPlainText("123");
    display->moveCursor(QTextCursor::End);
    display->setFocus();
    QTest::keyClick(display, Qt::Key_Backspace);

    QCOMPARE(display->toPlainText(), QString("12"));
}

void CalculatorGuiTests::keyboardDecimalAndParentheses()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    QVERIFY(display);

    display->setFocus();
    QTest::keyClicks(display, "(1.5+0.5)");
    QCOMPARE(display->toPlainText(), QString("(1.5+0.5)"));

    QTest::keyClick(display, Qt::Key_Return);
    QCOMPARE(display->toPlainText(), QString("2"));
}

void CalculatorGuiTests::keyboardNegativeSign()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    QVERIFY(display);

    display->setFocus();
    QTest::keyClicks(display, "-5+2");
    QCOMPARE(display->toPlainText(), QString("-5+2"));

    QTest::keyClick(display, Qt::Key_Return);
    QCOMPARE(display->toPlainText(), QString("-3"));
}

void CalculatorGuiTests::keyboardEnterEvaluates()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    QVERIFY(display);

    display->setFocus();
    QTest::keyClicks(display, "12+3");
    QCOMPARE(display->toPlainText(), QString("12+3"));

    QTest::keyClick(display, Qt::Key_Enter);
    QCOMPARE(display->toPlainText(), QString("15"));
}


void CalculatorGuiTests::angleModeCanToggleBackToDegrees()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *mode = window.findChild<QPushButton *>("pushButton_changeDegreesMode");
    auto *sin = window.findChild<QPushButton *>("pushButton_sin");
    auto *pi = window.findChild<QPushButton *>("pushButton_pi");
    auto *divide = window.findChild<QPushButton *>("pushButton_divided");
    auto *two = window.findChild<QPushButton *>("pushButton_2");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");

    QVERIFY(display);
    QVERIFY(mode);
    QVERIFY(sin);
    QVERIFY(pi);
    QVERIFY(divide);
    QVERIFY(two);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(clear);

    QCOMPARE(mode->text(), QString("grad"));

    // Switch to radians and evaluate sin(pi/2).
    QTest::mouseClick(mode, Qt::LeftButton);
    QCOMPARE(mode->text(), QString("rad"));
    QTest::mouseClick(sin, Qt::LeftButton);
    QTest::mouseClick(pi, Qt::LeftButton);
    QTest::mouseClick(divide, Qt::LeftButton);
    QTest::mouseClick(two, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - 1.0) < 1e-12);

    // Switch back to degrees and verify the original interpretation is restored.
    QTest::mouseClick(mode, Qt::LeftButton);
    QCOMPARE(mode->text(), QString("grad"));
    QTest::mouseClick(clear, Qt::LeftButton);
    QTest::mouseClick(sin, Qt::LeftButton);
    QTest::mouseClick(window.findChild<QPushButton *>("pushButton_9"), Qt::LeftButton);
    QTest::mouseClick(window.findChild<QPushButton *>("pushButton_0"), Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("1"));
}

void CalculatorGuiTests::inverseTrigonometricFunctionsInRadians()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *mode = window.findChild<QPushButton *>("pushButton_changeDegreesMode");
    auto *inverse = window.findChild<QPushButton *>("pushButton_inv");
    auto *sin = window.findChild<QPushButton *>("pushButton_sin");
    auto *cos = window.findChild<QPushButton *>("pushButton_cos");
    auto *tan = window.findChild<QPushButton *>("pushButton_tan");
    auto *one = window.findChild<QPushButton *>("pushButton_1");
    auto *zero = window.findChild<QPushButton *>("pushButton_0");
    auto *right = window.findChild<QPushButton *>("pushButton_bracket_right");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");

    QVERIFY(display);
    QVERIFY(mode);
    QVERIFY(inverse);
    QVERIFY(sin);
    QVERIFY(cos);
    QVERIFY(tan);
    QVERIFY(one);
    QVERIFY(zero);
    QVERIFY(right);
    QVERIFY(equals);
    QVERIFY(clear);

    QTest::mouseClick(mode, Qt::LeftButton);
    QCOMPARE(mode->text(), QString("rad"));
    QTest::mouseClick(inverse, Qt::LeftButton);

    // In radians, inverse trigonometric functions return radian values.
    QTest::mouseClick(sin, Qt::LeftButton);
    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - M_PI_2) < 1e-5);

    QTest::mouseClick(clear, Qt::LeftButton);
    QTest::mouseClick(cos, Qt::LeftButton);
    QTest::mouseClick(zero, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - M_PI_2) < 1e-5);

    QTest::mouseClick(clear, Qt::LeftButton);
    QTest::mouseClick(tan, Qt::LeftButton);
    QTest::mouseClick(one, Qt::LeftButton);
    QTest::mouseClick(right, Qt::LeftButton);
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - M_PI_4) < 1e-5);
}


void CalculatorGuiTests::negativeExponentAndNestedExpressions()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");
    QVERIFY(display);
    QVERIFY(equals);
    QVERIFY(clear);

    display->setPlainText("2^-3");
    QTest::mouseClick(equals, Qt::LeftButton);
    QVERIFY(qAbs(display->toPlainText().toDouble() - 0.125) < 1e-12);

    QTest::mouseClick(clear, Qt::LeftButton);
    display->setPlainText("(2+(3*(4+1)))");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("17"));

    QTest::mouseClick(clear, Qt::LeftButton);
    display->setPlainText("2^3^2");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("512"));
}

void CalculatorGuiTests::factorialBoundaryErrors()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");
    QVERIFY(display);
    QVERIFY(equals);
    QVERIFY(clear);

    display->setPlainText("0!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("1"));

    QTest::mouseClick(clear, Qt::LeftButton);
    display->setPlainText("(-3)!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("Factorial requires a non-negative integer"));

    QTest::mouseClick(clear, Qt::LeftButton);
    display->setPlainText("3.5!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("Factorial requires a non-negative integer"));

    QTest::mouseClick(clear, Qt::LeftButton);
    display->setPlainText("171!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("Factorial result is out of range"));

    // The display must recover after a factorial domain/overflow error.
    QTest::mouseClick(clear, Qt::LeftButton);
    display->setPlainText("6!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("720"));
}

void CalculatorGuiTests::largeNumberOverflowErrors()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    auto *clear = window.findChild<QPushButton *>("pushButton_clear");
    QVERIFY(display);
    QVERIFY(equals);
    QVERIFY(clear);

    display->setPlainText("1e308*1e308");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("Mathematical result is out of range"));

    QTest::mouseClick(clear, Qt::LeftButton);
    display->setPlainText("1e309");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("Numeric literal is out of range"));

    // A subsequent ordinary expression must still evaluate correctly.
    QTest::mouseClick(clear, Qt::LeftButton);
    display->setPlainText("10^2");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("100"));
}


void CalculatorGuiTests::repeatedFactorialAndSignPrecedence()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    QVERIFY(display);
    QVERIFY(equals);

    display->setPlainText("3!!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("720"));

    display->setPlainText("-3!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("-6"));

    display->setPlainText("2^3!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("64"));

    display->setPlainText("(-3)!");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(),
             QString("Factorial requires a non-negative integer"));
}

void CalculatorGuiTests::malformedScientificNotationInDisplay()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    QVERIFY(display);
    QVERIFY(equals);

    const QStringList malformedLiterals = {"1e", "1e+", "2.5e-"};
    for (const QString &literal : malformedLiterals) {
        display->setPlainText(literal);
        QTest::mouseClick(equals, Qt::LeftButton);
        QCOMPARE(display->toPlainText(), QString("Unexpected character: e"));
    }

    display->setPlainText("1.25e2");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("125"));
}

void CalculatorGuiTests::parserNestingLimitInDisplay()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");
    QVERIFY(display);
    QVERIFY(equals);

    const QString withinLimit = QString(100, '(') + "1" + QString(100, ')');
    display->setPlainText(withinLimit);
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("1"));

    const QString beyondLimit = QString(300, '(') + "1" + QString(300, ')');
    display->setPlainText(beyondLimit);
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("Expression nesting limit exceeded"));
}


void CalculatorGuiTests::guiBoundaryBranches()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *cursorLeft = window.findChild<QPushButton *>("pushButton_cursor_left");
    auto *cursorRight = window.findChild<QPushButton *>("pushButton_cursor_right");
    auto *backspace = window.findChild<QPushButton *>("pushButton_erase_left");
    auto *historyBack = window.findChild<QPushButton *>("pushButton_history_back");
    auto *historyForward = window.findChild<QPushButton *>("pushButton_history_forward");
    auto *reciprocal = window.findChild<QPushButton *>("pushButton_edx");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(cursorLeft);
    QVERIFY(cursorRight);
    QVERIFY(backspace);
    QVERIFY(historyBack);
    QVERIFY(historyForward);
    QVERIFY(reciprocal);
    QVERIFY(equals);

    // Empty history is a no-op in both directions.
    QTest::mouseClick(historyBack, Qt::LeftButton);
    QTest::mouseClick(historyForward, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString());

    // Backspace at the start of the editor must not remove any character.
    display->setPlainText("12");
    display->moveCursor(QTextCursor::Start);
    QTest::mouseClick(backspace, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("12"));

    // Exercise the right-cursor action and insert at the resulting position.
    QTest::mouseClick(cursorRight, Qt::LeftButton);
    QTest::mouseClick(cursorLeft, Qt::LeftButton);
    QTest::mouseClick(cursorRight, Qt::LeftButton);
    QTest::mouseClick(window.findChild<QPushButton *>("pushButton_3"), Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("132"));

    // The error branch clears the error instead of deleting a character from it.
    display->setPlainText("1/0");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("Division by zero"));
    QTest::mouseClick(backspace, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString());

    // Exercise the integer formatting branch in the reciprocal button handler.
    display->setPlainText("1");
    QTest::mouseClick(reciprocal, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("1"));
}


void CalculatorGuiTests::guiSignAndEventFilterBoundaries()
{
    MainWindow window;
    window.show();

    auto *display = window.findChild<QTextEdit *>("textEdit");
    auto *sign = window.findChild<QPushButton *>("pushButton_plusminus");
    auto *equals = window.findChild<QPushButton *>("pushButton_equals");

    QVERIFY(display);
    QVERIFY(sign);
    QVERIFY(equals);

    // Toggling the sign on an empty expression is a no-op.
    QTest::mouseClick(sign, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString());

    // After an evaluation error, the sign button first clears the error.
    display->setPlainText("1/0");
    QTest::mouseClick(equals, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString("Division by zero"));
    QTest::mouseClick(sign, Qt::LeftButton);
    QCOMPARE(display->toPlainText(), QString());

    // Non-Return key events must pass through the event filter.
    display->setPlainText("12");
    display->setFocus();
    QTest::keyClick(display, Qt::Key_Left);
    QCOMPARE(display->textCursor().position(), 1);
    QTest::keyClick(display, Qt::Key_Right);
    QCOMPARE(display->textCursor().position(), 2);
}

QTEST_MAIN(CalculatorGuiTests)

#include "Test_MainWindow.moc"
