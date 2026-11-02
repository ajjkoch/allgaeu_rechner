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

QTEST_MAIN(CalculatorGuiTests)

#include "Test_MainWindow.moc"
