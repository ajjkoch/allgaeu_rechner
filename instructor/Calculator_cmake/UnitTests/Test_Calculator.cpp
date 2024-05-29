#include <QtTest>

#include <sstream> // for std::stringstream
#include <iomanip> // for std::setprecision

#include "../../../common/Util.h"

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
    void test_pi();
    void test_sin();
    void test_cos();
    void test_tan();
    void test_sqrt();
    void test_power();
    void test_log();
    void test_ln();
    void test_expression();
};

CalculatorTests::CalculatorTests() {}

CalculatorTests::~CalculatorTests() {}

void CalculatorTests::test_addition()
{
    double res = Calculator::evaluate("3+4");
    QCOMPARE(res, 7);
}

void CalculatorTests::test_subtraction()
{
    double res = Calculator::evaluate("3-4");
    QCOMPARE(res, -1);
}

void CalculatorTests::test_multiplication()
{
    double res = Calculator::evaluate("3*4");
    QCOMPARE(res, 12);
}

void CalculatorTests::test_divition()
{
    double res = Calculator::evaluate("4/2.0");
    QCOMPARE(res, 2.0);
}

void CalculatorTests::test_pi()
{
    {
        std::string expression = "pi";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 3.14159265359);
    }
    {
        std::string expression = "2*pi";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 6.28318530718);
    }
    {
        std::string expression = "2pi";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 6.28318530718);
    }
    {
        std::string expression = "pi/2";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 1.5707963267948966);
    }
    {
        std::string expression = "3pi/2";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 4.71238898038469);
    }
    {
        std::string expression = "3/2*pi";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 4.71238898038469);
    }
    {
        std::string expression = "3/2pi";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 0.477464829275686);
    }
}

void CalculatorTests::test_sin()
{
    {
        std::string expression = "sin(180)";
        double result = Calculator::evaluate(expression);
        //QCOMPARE(result, 0); // this fails because it compares an integer to a fractional value.
        QCOMPARE(result, 0.0); // so use a fractional value for the compare
    }
    {
        std::string expression = "sin(90)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 1.0);
    }
    {
        std::string expression = "sin(45)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, std::sqrt(2)/2.0);
    }
}

void CalculatorTests::test_cos()
{
    {
        std::string expression = "cos(0)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 1.0); // so use a fractional value for the compare
    }
}

void CalculatorTests::test_tan()
{
    {
        std::string expression = "tan(60)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, std::sqrt(3)); // so use a fractional value for the compare
    }
}

void CalculatorTests::test_sqrt()
{
    {
        std::string expression = "sqrt(9)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 3);
    }
    {
        std::string expression = "sqrt(100)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 10);
    }
    {
        std::string expression = "sqrt(2)";
        double expected = 1.414213562;
        double result = Calculator::evaluate(expression);
        //QCOMPARE(result, expected); // does not work due to floating point errors // fuzzy compare, always false. compares floats.
        //QVERIFY(std::abs(result - expected) < 1e-9); // Adjust tolerance as needed // lost actual and expected value
        //QVERIFY2(compareDoubles(result, expected), ("Actual: " + std::to_string(result) + ", Expected: " + std::to_string(expected)).c_str()); // less precision. only 6 digits
        std::stringstream message;
        message << "Actual: " << std::fixed << std::setprecision(9) << result << ", Expected: " << std::fixed << std::setprecision(9) << expected;
        QVERIFY2( compareDoubles(result, expected), message.str().c_str() );
    }
}

void CalculatorTests::test_power()
{
    {
        std::string expression = "9^3";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 729);
    }
    {
        std::string expression = "3^4";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 81);
    }
    {
        std::string expression = "10^9";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 1000000000);
    }
    {
        std::string expression = "10^(-9)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 0.000000001);
    }
    {
        std::string expression = "10^-9";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 0.000000001);
    }
}

void CalculatorTests::test_log()
{
    {
        std::string expression = "log(10)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 1);
    }
}

void CalculatorTests::test_ln()
{
    {
        std::string expression = "ln(1)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 0);
    }
}

void CalculatorTests::test_expression()
{
    {
        std::string expression = "-1+pi/2+3";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 3.5707963267948966);
    }
    {
        std::string expression = "(((2+3)/5)*7)^2";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 49);
    }
    {
        std::string expression = "((1.41415 + 10) / 3.457) - 5";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, -1.6982499276829621);
    }
    {
        std::string expression = "(((2+3)/5)*7)^2 + ln(10) + sin(90)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 52.302585093);
    }
    {
        std::string expression = "(((2+3)/5)*7)^2 + log(10) + sin(90)-cos(0)+tan(60)";
        double result = Calculator::evaluate(expression);
        QCOMPARE(result, 50+std::sqrt(3));
    }
}

/*/
void TestQString::Test_QtCalculator()
{
    {
        QString expression = "(((2+3)/5)*7)^2";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 49);
    }
    {
        QString expression = "((1.41415 + 10) / 3.457) - 5";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, -1.6982499276829621);
    }
    {
        QString expression = "9^3";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 729);
    }
    {
        QString expression = "3^4";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 81);
    }
    {
        QString expression = "10^9";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 1000000000);
    }
    {
        QString expression = "10^(-9)";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 0.000000001);
    }
    {
        QString expression = "10^-9";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 0.000000001);
    }
    {
        QString expression = "(((2+3)/5)*7)^2 + ln(10) + sin(90)";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 52.302585093);
    }
    {
        QString expression = "(((2+3)/5)*7)^2 + log(10) + sin(90)";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 51);
    }
    {
        QString expression = "sin(180)";
        double result = QtCalculator::evaluate(expression);
        //QCOMPARE(result, 0); // this fails because it compares an integer to a fractional value.
        QCOMPARE(result, 0.0); // so use a fractional value for the compare
    }
    {
        QString expression = "sin(90)";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 1.0);
    }
    {
        QString expression = "log(10)";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 1);
    }
    {
        QString expression = "sqrt(9)";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 3);
    }
    {
        QString expression = "sqrt(100)";
        double result = QtCalculator::evaluate(expression);
        QCOMPARE(result, 10);
    }
    {
        QString expression = "sqrt(2)";
        double expected = 1.414213562;
        double result = QtCalculator::evaluate(expression);
        //QCOMPARE(result, expected); // does not work due to floating point errors // fuzzy compare, always false. compares floats.
        //QVERIFY(std::abs(result - expected) < 1e-9); // Adjust tolerance as needed // lost actual and expected value
        //QVERIFY2(compareDoubles(result, expected), ("Actual: " + std::to_string(result) + ", Expected: " + std::to_string(expected)).c_str()); // less precision. only 6 digits
        std::stringstream message;
        message << "Actual: " << std::fixed << std::setprecision(9) << result << ", Expected: " << std::fixed << std::setprecision(9) << expected;
        QVERIFY2(compareDoubles(result, expected), message.str().c_str());
    }
}
//*/

QTEST_APPLESS_MAIN(CalculatorTests)

#include "Test_Calculator.moc"
