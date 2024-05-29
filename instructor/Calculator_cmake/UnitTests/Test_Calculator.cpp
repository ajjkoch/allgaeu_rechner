#include <QtTest>
#include "../Util.h"

class CalculatorTests : public QObject
{
    Q_OBJECT

public:
    CalculatorTests();
    ~CalculatorTests();

private slots:
    void test_addition();
    void test_subtraction();
    void test_multiplication();
    void test_divition();
};

CalculatorTests::CalculatorTests() {}

CalculatorTests::~CalculatorTests() {}

void CalculatorTests::test_addition()
{
    double res = calculate(3,4,ADD);
    QCOMPARE(res, 7);
}

void CalculatorTests::test_subtraction()
{
    double res = calculate(3,4,SUB);
    QCOMPARE(res, -1);
}

void CalculatorTests::test_multiplication()
{
    double res = calculate(3,4,MULT);
    QCOMPARE(res, 12);
}

void CalculatorTests::test_divition()
{
    double res = calculate(4,4,DIV);
    QCOMPARE(res, 1);
}

QTEST_APPLESS_MAIN(CalculatorTests)

#include "Test_Calculator.moc"
