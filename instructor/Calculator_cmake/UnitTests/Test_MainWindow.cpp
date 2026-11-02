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

QTEST_MAIN(CalculatorGuiTests)

#include "Test_MainWindow.moc"
