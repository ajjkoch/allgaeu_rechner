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
    QVERIFY(display->toPlainText() != QString("1/0"));

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
    QVERIFY(display->toPlainText() != QString("sqrt(-9)"));
    QVERIFY(!display->toPlainText().isEmpty());

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
    QVERIFY(display->toPlainText() != QString("tan(90)"));
    QVERIFY(!display->toPlainText().isEmpty());

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
    QVERIFY(display->toPlainText() != QString("asin(2)"));
    QVERIFY(!display->toPlainText().isEmpty());
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
    QVERIFY(display->toPlainText() != QString("0"));
    QVERIFY(!display->toPlainText().isEmpty());
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

QTEST_MAIN(CalculatorGuiTests)

#include "Test_MainWindow.moc"
