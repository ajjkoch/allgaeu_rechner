#include <QtTest>

#include <sstream> // for std::stringstream
#include <iomanip> // for std::setprecision
#include <vector>
#include <limits>

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
    void test_trigonometric_edge_cases();
    void test_sqrt();
    void test_power();
    void test_power_edge_cases();
    void test_factorial_boundaries();
    void test_log();
    void test_ln();
    void test_percent();
    void test_faculty();
    void test_expression();
    void test_operator_precedence();
    void test_modulo();
    void test_invalid_expressions();
    void test_domain_errors();
    void test_math_function_boundaries();
    void test_calculator_instances_are_independent();
    void test_numeric_literals();
    void test_invalid_numeric_literals_and_implicit_multiplication();
    void test_numeric_overflow();
    void test_numeric_underflow();
    void test_small_nonzero_operands();
    void test_arithmetic_overflow_and_underflow();

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


void CalculatorTests::test_trigonometric_edge_cases()
{
    m_calculator.setDegMode(true);

    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("tan(90)"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("tan(270)"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("tan(-90)"), std::domain_error);

    QVERIFY(std::fabs(m_calculator.evaluate("sin(180)")) < 1e-12);
    QVERIFY(std::fabs(m_calculator.evaluate("cos(90)")) < 1e-12);
    QVERIFY(std::fabs(m_calculator.evaluate("sin(360000000180)")) < 1e-12);

    QCOMPARE(m_calculator.evaluate("asin(1)"), 90.0);
    QCOMPARE(m_calculator.evaluate("asin(-1)"), -90.0);
    QCOMPARE(m_calculator.evaluate("acos(1)"), 0.0);
    QCOMPARE(m_calculator.evaluate("acos(-1)"), 180.0);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("asin(1.0000000001)"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("acos(-1.0000000001)"), std::domain_error);
    QVERIFY(std::fabs(m_calculator.evaluate("atan(1)") - 45.0) < 1e-12);

    m_calculator.setDegMode(false);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("tan(pi/2)"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("tan(3*pi/2)"), std::domain_error);
    QVERIFY(std::fabs(m_calculator.evaluate("sin(pi)")) < 1e-12);
    QVERIFY(std::fabs(m_calculator.evaluate("cos(pi/2)")) < 1e-12);
    QCOMPARE(m_calculator.evaluate("asin(1)"), std::acos(-1.0) / 2.0);
    QCOMPARE(m_calculator.evaluate("acos(-1)"), std::acos(-1.0));
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


void CalculatorTests::test_power_edge_cases()
{
    QCOMPARE(m_calculator.evaluate("(-2)^3"), -8.0);
    QCOMPARE(m_calculator.evaluate("(-2)^4"), 16.0);

    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("(-2)^0.5"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("0^-1"), std::domain_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("0^(-2)"), std::domain_error);

    QCOMPARE(m_calculator.evaluate("10^-400"), 0.0);
    QCOMPARE(m_calculator.evaluate("(-10)^-400"), 0.0);
}

void CalculatorTests::test_factorial_boundaries()
{
    const double factorial170 = m_calculator.evaluate("170!");
    QVERIFY(std::isfinite(factorial170));
    QVERIFY(factorial170 > 0.0);

    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("171!"), std::overflow_error);
    QCOMPARE(m_calculator.evaluate("0!"), 1.0);
    QCOMPARE(m_calculator.evaluate("1!"), 1.0);
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

void CalculatorTests::test_math_function_boundaries()
{
    // sqrt: zero, ordinary values and positive subnormal input.
    QCOMPARE(m_calculator.evaluate("sqrt(0)"), 0.0);
    QCOMPARE(m_calculator.evaluate("sqrt(4)"), 2.0);
    const double tinyRoot = m_calculator.evaluate("sqrt(1e-320)");
    QVERIFY(tinyRoot > 0.0);
    QVERIFY(std::isfinite(tinyRoot));

    // ln/log: smallest positive normal and subnormal values remain valid.
    QVERIFY(std::isfinite(m_calculator.evaluate("ln(1e-308)")));
    QVERIFY(std::isfinite(m_calculator.evaluate("ln(1e-320)")));
    QVERIFY(std::isfinite(m_calculator.evaluate("log(1e-308)")));
    QVERIFY(std::isfinite(m_calculator.evaluate("log(1e-320)")));
    QCOMPARE(m_calculator.evaluate("ln(1)"), 0.0);
    QCOMPARE(m_calculator.evaluate("log(1)"), 0.0);

    // exp: finite near the upper limit, overflow above it, underflow below it.
    QVERIFY(std::isfinite(m_calculator.evaluate("exp(709)")));
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("exp(710)"), std::overflow_error);
    QCOMPARE(m_calculator.evaluate("exp(-1000)"), 0.0);

    // edx is the reciprocal: finite for suitable non-zero values,
    // overflow for a very small non-zero value.
    QCOMPARE(m_calculator.evaluate("edx(2)"), 0.5);
    QCOMPARE(m_calculator.evaluate("edx(-4)"), -0.25);
    QVERIFY(std::isfinite(m_calculator.evaluate("edx(1e-308)")));
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("edx(1e-309)"), std::overflow_error);
    QVERIFY(std::isfinite(m_calculator.evaluate("edx(1e308)")));
}

QTEST_APPLESS_MAIN(CalculatorTests)


void CalculatorTests::test_numeric_literals()
{
    QCOMPARE(m_calculator.evaluate("1.5e3"), 1500.0);
    QCOMPARE(m_calculator.evaluate("2E-4"), 0.0002);
    QCOMPARE(m_calculator.evaluate(".5"), 0.5);
    QCOMPARE(m_calculator.evaluate("1."), 1.0);
    QCOMPARE(m_calculator.evaluate("2pi"), 2.0 * std::acos(-1.0));
}

void CalculatorTests::test_invalid_numeric_literals_and_implicit_multiplication()
{
    const std::vector<std::string> invalidExpressions = {
        ".", "1.2.3", "1e", "1e+", "2 3", "2(3+4)", "pi(2)"
    };

    for (const std::string& expression : invalidExpressions) {
        QVERIFY2(
            [&]() {
                try {
                    m_calculator.evaluate(expression);
                    return false;
                } catch (const std::invalid_argument&) {
                    return true;
                } catch (...) {
                    return false;
                }
            }(),
            ("Expected invalid_argument for: " + expression).c_str());
    }
}

void CalculatorTests::test_numeric_overflow()
{
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("1e309"), std::overflow_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("10^309"), std::overflow_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate("171!"), std::overflow_error);
}

void CalculatorTests::test_numeric_underflow()
{
    QCOMPARE(m_calculator.evaluate("1e-400"), 0.0);
    QCOMPARE(m_calculator.evaluate("-1e-400"), 0.0);

    const double subnormal = m_calculator.evaluate("1e-320");
    QVERIFY(subnormal > 0.0);
    QVERIFY(subnormal < std::numeric_limits<double>::min());
}

void CalculatorTests::test_small_nonzero_operands()
{
    QCOMPARE(m_calculator.evaluate("1e-300/1e-300"), 1.0);
    QVERIFY(std::fabs(m_calculator.evaluate("5e-300 mod 2e-300") / 1e-300 - 1.0) < 1e-12);

    m_calculator.setInverted(true);
    QCOMPARE(m_calculator.evaluate("1e-300%1e-300"), 100.0);
    m_calculator.setInverted(false);
    QVERIFY(std::fabs(m_calculator.evaluate("1e-150%1e-150") / 1e-302 - 1.0) < 1e-12);
}

void CalculatorTests::test_arithmetic_overflow_and_underflow()
{
    const std::string max = "1.7976931348623157e308";

    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate(max + "+" + max), std::overflow_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate(max + "-(-" + max + ")"), std::overflow_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate(max + "*2"), std::overflow_error);
    QVERIFY_EXCEPTION_THROWN(m_calculator.evaluate(max + "/1e-308"), std::overflow_error);

    // Underflow to zero is accepted when the exact result is too small
    // to be represented by a double.
    QCOMPARE(m_calculator.evaluate("1e-300*1e-300"), 0.0);
    QCOMPARE(m_calculator.evaluate("1e-300/1e300"), 0.0);

    // The percentage result is representable even though dividing the
    // smaller operand by 100 first would underflow it to zero.
    QVERIFY(std::fabs(m_calculator.evaluate("1e308%1e-320") / 1e-14 - 1.0) < 1e-12);
    QVERIFY(std::fabs(m_calculator.evaluate("1e-320%1e308") / 1e-14 - 1.0) < 1e-12);
}

void CalculatorTests::test_calculator_instances_are_independent()
{
    Calculator degreesCalculator;
    Calculator radiansCalculator;

    degreesCalculator.setDegMode(true);
    radiansCalculator.setDegMode(false);

    QVERIFY(std::fabs(degreesCalculator.evaluate("sin(90)") - 1.0) < 1e-12);
    QVERIFY(std::fabs(radiansCalculator.evaluate("sin(90)") - std::sin(90.0)) < 1e-12);

    degreesCalculator.setInverted(true);
    radiansCalculator.setInverted(false);

    QCOMPARE(degreesCalculator.evaluate("15%15"), 100.0);
    QCOMPARE(radiansCalculator.evaluate("15%15"), 2.25);
}

#include "Test_Calculator.moc"
