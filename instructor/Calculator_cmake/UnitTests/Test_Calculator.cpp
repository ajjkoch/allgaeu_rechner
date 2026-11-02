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
    void init();
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
    void test_percent();
    void test_faculty();
    void test_expression();
    void test_operator_precedence();
    void test_modulo();
    void test_invalid_expressions();
    void test_domain_errors();

private:
    Calculator m_calculator;
};

CalculatorTests::CalculatorTests() {}

CalculatorTests::~CalculatorTests() {}

void CalculatorTests::init()
{
    m_calculator.setDegMode(true);
    m_calculator.setInverted(false);
}

void CalculatorTests::test_addition()
{
    double res = m_calculator.evaluate("3+4");
    QCOMPARE(res, 7);
}

void CalculatorTests::test_percent()
{
    m_calculator.setInverted(false);

    {
        double res = m_calculator.evaluate("100%15");
        QCOMPARE(res, 15);
    }
    {
        double res = m_calculator.evaluate("551%10");
        QCOMPARE(res, 55.1);
    }
    {
        double res = m_calculator.evaluate("551%1");
        QCOMPARE(res, 5.51);
    }

    m_calculator.setInverted(true);

    {
        double res = m_calculator.evaluate("15%15");
        QCOMPARE(res, 100);
    }
    {
        double res = m_calculator.evaluate(" 5.51%1");
        QCOMPARE(res, 551);
    }

    m_calculator.setInverted(false);
}

void CalculatorTests::test_subtraction()
{
    double res = m_calculator.evaluate("3-4");
    QCOMPARE(res, -1);
}

void CalculatorTests::test_multiplication()
{
    double res = m_calculator.evaluate("3*4");
    QCOMPARE(res, 12);
}

void CalculatorTests::test_divition()
{
    double res = m_calculator.evaluate("4/2.0");
    QCOMPARE(res, 2.0);
}

void CalculatorTests::test_faculty()
{
    {
        std::string expression = "3!";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 6);
    }
    {
        std::string expression = "7!";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 5040);
    }
    {
        std::string expression = "12!";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 479001600);
    }
    {
        std::string expression = "-1!";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, -1);
    }
    {
        std::string expression = "26!";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 4.032914611266057e26);
    }
}

void CalculatorTests::test_pi()
{
    {
        std::string expression = "pi";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 3.14159265359);
    }
    {
        std::string expression = "2*pi";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 6.28318530718);
    }
    {
        std::string expression = "2pi";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 6.28318530718);
    }
    {
        std::string expression = "pi/2";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 1.5707963267948966);
    }
    {
        std::string expression = "3pi/2";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 4.71238898038469);
    }
    {
        std::string expression = "3/2*pi";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 4.71238898038469);
    }
    {
        std::string expression = "3/2pi";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 0.477464829275686);
    }
}

void CalculatorTests::test_sin()
{
    m_calculator.setDegMode(true);

    {
        std::string expression = "sin(180)";
        double result = m_calculator.evaluate(expression);
        //QCOMPARE(result, 0); // this fails because it compares an integer to a fractional value.
        QVERIFY(std::fabs(result) < 1e-12); // trigonometric round-off near zero
    }
    {
        std::string expression = "sin(90)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 1.0);
    }
    {
        std::string expression = "sin(45)";
        double result = m_calculator.evaluate(expression);
        QVERIFY(std::fabs(result - std::sqrt(2)/2.0) < 1e-12);
    }

    m_calculator.setDegMode(false);

    {
        std::string expression = "sin(pi)";
        double result = m_calculator.evaluate(expression);
        //QCOMPARE(result, 0); // this fails because it compares an integer to a fractional value.
        QCOMPARE(result, 0.0); // so use a fractional value for the compare
    }
    {
        std::string expression = "sin(pi/2)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 1.0);
    }
    {
        std::string expression = "sin(pi/4)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, std::sqrt(2)/2.0);
    }

    m_calculator.setDegMode(false);
}

void CalculatorTests::test_cos()
{
    m_calculator.setDegMode(true);

    {
        std::string expression = "cos(90)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 0.0); // so use a fractional value for the compare
    }

    m_calculator.setDegMode(false);

    {
        std::string expression = "cos(pi)";
        double result = m_calculator.evaluate(expression);
        QVERIFY(std::fabs(result + 1.0) < 1e-12);
    }
}

void CalculatorTests::test_tan()
{
    m_calculator.setDegMode(true);

    {
        std::string expression = "tan(60)";
        double result = m_calculator.evaluate(expression);
        QVERIFY(std::fabs(result - std::sqrt(3)) < 1e-12);
    }

    m_calculator.setDegMode(false);

    {
        std::string expression = "tan(pi/3)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, std::sqrt(3)); // so use a fractional value for the compare
    }
}

void CalculatorTests::test_sqrt()
{
    {
        std::string expression = "sqrt(9)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 3);
    }
    {
        std::string expression = "sqrt(100)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 10);
    }
    {
        std::string expression = "sqrt(2)";
        double expected = 1.414213562;
        double result = m_calculator.evaluate(expression);
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
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 729);
    }
    {
        std::string expression = "3^4";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 81);
    }
    {
        std::string expression = "10^9";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 1000000000);
    }
    {
        std::string expression = "10^(-9)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 0.000000001);
    }
    {
        std::string expression = "10^-9";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 0.000000001);
    }
}

void CalculatorTests::test_log()
{
    {
        std::string expression = "log(10)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 1);
    }
}

void CalculatorTests::test_ln()
{
    {
        std::string expression = "ln(1)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 0);
    }
}

void CalculatorTests::test_expression()
{

    m_calculator.setDegMode(true);

    {
        std::string expression = "-1+pi/2+3";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 3.5707963267948966);
    }
    {
        std::string expression = "(((2+3)/5)*7)^2";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 49);
    }
    {
        std::string expression = "((1.41415 + 10) / 3.457) - 5";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, -1.6982499276829621);
    }
    {
        std::string expression = "(((2+3)/5)*7)^2 + ln(10) + sin(90)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 52.302585093);
    }
    {
        std::string expression = "(((2+3)/5)*7)^2 + log(10) + sin(90)-cos(0)+tan(60)";
        double result = m_calculator.evaluate(expression);
        QCOMPARE(result, 50+std::sqrt(3));
    }
}


void CalculatorTests::test_operator_precedence()
{
    QCOMPARE(m_calculator.evaluate("-2^2"), -4.0);
    QCOMPARE(m_calculator.evaluate("(-2)^2"), 4.0);
    QCOMPARE(m_calculator.evaluate("2^-2"), 0.25);
    QCOMPARE(m_calculator.evaluate("2^3^2"), 512.0);
}

void CalculatorTests::test_modulo()
{
    QCOMPARE(m_calculator.evaluate("5 mod 2"), 1.0);
    QCOMPARE(m_calculator.evaluate("5 mod 2 + 1"), 2.0);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("5 mod 0"), std::domain_error);
}

void CalculatorTests::test_invalid_expressions()
{
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate(""), std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("2+3abc"), std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("()"), std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("(2+3"), std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("2+"), std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("unknown(2)"), std::invalid_argument);
}

void CalculatorTests::test_domain_errors()
{
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("1/0"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("sqrt(-1)"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("ln(0)"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("log(-1)"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("asin(2)"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("5.5!"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("(-1)!"), std::domain_error);
}

QTEST_APPLESS_MAIN(CalculatorTests)

#include "Test_Calculator.moc"
