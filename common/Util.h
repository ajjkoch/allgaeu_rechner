#ifndef UTILS_COMMON_H
#define UTILS_COMMON_H

#include <cerrno>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

// Compatibility helpers used by older code and tests.
static double sie_mod(double x, double y)
{
    if (y == 0.0) {
        throw std::domain_error("Modulo by zero");
    }
    return std::fmod(x, y);
}

static double facultyRecursive(int n)
{
    if (n < 0) {
        throw std::domain_error("Factorial is undefined for negative numbers");
    }

    double result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= i;
        if (!std::isfinite(result)) {
            throw std::overflow_error("Factorial result is out of range");
        }
    }
    return result;
}

static bool compareDoubles(double a, double b, double epsilon = 1e-9)
{
    return std::fabs(a - b) <= epsilon;
}

/**
 * A small recursive-descent expression evaluator.
 *
 * Supported operators:
 *   + - * /       arithmetic
 *   mod           floating-point remainder
 *   %             percentage (a % b means a * b / 100)
 *   ^             right-associative exponentiation
 *   !             factorial
 *
 * Supported functions:
 *   sqrt, sin, cos, tan, asin, acos, atan, log, ln, exp, edx
 *
 * Trigonometric functions use degrees by default. Call setDegMode(false)
 * to switch to radians. The percentage key can be inverted with setInverted.
 */
class Calculator
{
public:
    long double evaluate(const std::string& expression) const
    {
        Parser parser(expression, m_degrees, m_inverted);
        return parser.parse();
    }

    void setDegMode(bool isDegrees)
    {
        m_degrees = isDegrees;
    }

    void setInverted(bool isInverted)
    {
        m_inverted = isInverted;
    }

private:
    bool m_degrees = true;
    bool m_inverted = false;

    class Parser
    {
    public:
        Parser(const std::string& expression, bool degrees, bool inverted)
            : m_expression(expression),
              m_position(0),
              m_degrees(degrees),
              m_inverted(inverted)
        {
        }

        double parse()
        {
            skipWhitespace();
            if (atEnd()) {
                throw std::invalid_argument("Expression is empty");
            }

            const double result = parseAdditionSubtraction();
            skipWhitespace();

            if (!atEnd()) {
                throw std::invalid_argument(
                    "Unexpected character: " + std::string(1, current()));
            }
            return checked(result);
        }

    private:
        const std::string& m_expression;
        std::size_t m_position;
        bool m_degrees;
        bool m_inverted;

        bool atEnd() const
        {
            return m_position >= m_expression.size();
        }

        char current() const
        {
            return atEnd() ? '\0' : m_expression[m_position];
        }

        void skipWhitespace()
        {
            while (!atEnd() &&
                   std::isspace(static_cast<unsigned char>(current()))) {
                ++m_position;
            }
        }

        bool consume(char expected)
        {
            skipWhitespace();
            if (current() != expected) {
                return false;
            }
            ++m_position;
            return true;
        }

        bool consumeWord(const std::string& word)
        {
            skipWhitespace();
            if (m_expression.compare(m_position, word.size(), word) != 0) {
                return false;
            }

            const std::size_t end = m_position + word.size();
            if (end < m_expression.size() &&
                std::isalpha(static_cast<unsigned char>(m_expression[end]))) {
                return false;
            }

            m_position = end;
            return true;
        }

        static double checked(double value)
        {
            if (std::isinf(value)) {
                throw std::overflow_error("Mathematical result is out of range");
            }
            if (std::isnan(value)) {
                throw std::domain_error("Mathematical result is undefined");
            }
            return value;
        }

        double parseAdditionSubtraction()
        {
            double left = parseMultiplicationDivision();

            while (true) {
                if (consume('+')) {
                    left = checked(left + parseMultiplicationDivision());
                } else if (consume('-')) {
                    left = checked(left - parseMultiplicationDivision());
                } else {
                    return left;
                }
            }
        }

        double parseMultiplicationDivision()
        {
            double left = parseUnary();

            while (true) {
                if (consume('*')) {
                    left = checked(left * parseUnary());
                } else if (consume('/')) {
                    const double right = parseUnary();
                    if (right == 0.0) {
                        throw std::domain_error("Division by zero");
                    }
                    left = checked(left / right);
                } else if (consume('%')) {
                    const double right = parseUnary();
                    if (m_inverted) {
                        if (right == 0.0) {
                            throw std::domain_error("Percentage divisor is zero");
                        }
                        left = checked((left / right) * 100.0);
                    } else {
                        // Divide the operand with the larger magnitude first.
                        // This avoids premature underflow in right / 100 and
                        // reduces the risk of an overflowing intermediate product.
                        const double percentage = std::fabs(left) >= std::fabs(right)
                            ? (left / 100.0) * right
                            : left * (right / 100.0);
                        left = checked(percentage);
                    }
                } else if (consumeWord("mod")) {
                    const double right = parseUnary();
                    if (right == 0.0) {
                        throw std::domain_error("Modulo by zero");
                    }
                    left = checked(std::fmod(left, right));
                } else {
                    return left;
                }
            }
        }

        // Unary signs bind less tightly than exponentiation:
        // -2^2 is -(2^2), while 2^-2 is valid.
        double parseUnary()
        {
            if (consume('+')) {
                return parseUnary();
            }
            if (consume('-')) {
                return checked(-parseUnary());
            }
            return parsePower();
        }

        // Calling parseUnary for the exponent makes exponentiation
        // right-associative and permits negative exponents.
        double parsePower()
        {
            const double base = parsePostfix();
            if (consume('^')) {
                const double exponent = parseUnary();
                if (base == 0.0 && exponent < 0.0) {
                    throw std::domain_error(
                        "Zero cannot be raised to a negative power");
                }
                return checked(std::pow(base, exponent));
            }
            return base;
        }

        double parsePostfix()
        {
            double value = parsePrimary();
            while (consume('!')) {
                if (value < 0.0 || std::floor(value) != value) {
                    throw std::domain_error(
                        "Factorial requires a non-negative integer");
                }
                if (value > 170.0) {
                    throw std::overflow_error("Factorial result is out of range");
                }

                double result = 1.0;
                for (int i = 2; i <= static_cast<int>(value); ++i) {
                    result *= i;
                }
                value = checked(result);
            }
            return value;
        }

        double parsePrimary()
        {
            skipWhitespace();
            if (atEnd()) {
                throw std::invalid_argument("Unexpected end of expression");
            }

            if (consume('(')) {
                const double value = parseAdditionSubtraction();
                if (!consume(')')) {
                    throw std::invalid_argument("Mismatched parenthesis");
                }
                return value;
            }

            if (std::isdigit(static_cast<unsigned char>(current())) ||
                current() == '.') {
                const std::size_t start = m_position;
                const std::string remaining = m_expression.substr(start);
                char* end = nullptr;
                errno = 0;
                const double value = std::strtod(remaining.c_str(), &end);
                const std::size_t parsed =
                    static_cast<std::size_t>(end - remaining.c_str());

                if (parsed == 0) {
                    throw std::invalid_argument("Invalid numeric literal");
                }
                if (std::isinf(value)) {
                    throw std::overflow_error("Numeric literal is out of range");
                }
                // A finite result of zero or a subnormal value is an underflow,
                // which is valid IEEE-754 behavior for this calculator.
                m_position += parsed;

                // Preserve the existing calculator shorthand, e.g. 2pi.
                if (m_expression.compare(m_position, 2, "pi") == 0) {
                    m_position += 2;
                    return checked(value * pi());
                }
                return checked(value);
            }

            if (consumeWord("pi")) {
                return pi();
            }

            if (std::isalpha(static_cast<unsigned char>(current()))) {
                return parseFunction();
            }

            throw std::invalid_argument(
                "Invalid character: " + std::string(1, current()));
        }

        double parseFunction()
        {
            const std::size_t start = m_position;
            while (!atEnd() &&
                   std::isalpha(static_cast<unsigned char>(current()))) {
                ++m_position;
            }
            const std::string name = m_expression.substr(start, m_position - start);

            if (!consume('(')) {
                throw std::invalid_argument("Expected '(' after " + name);
            }

            const double argument = parseAdditionSubtraction();
            if (!consume(')')) {
                throw std::invalid_argument("Mismatched parenthesis in " + name);
            }

            if (name == "sqrt") {
                if (argument < 0.0) {
                    throw std::domain_error("sqrt requires a non-negative argument");
                }
                return checked(std::sqrt(argument));
            }
            if (name == "ln") {
                if (argument <= 0.0) {
                    throw std::domain_error("ln requires a positive argument");
                }
                return checked(std::log(argument));
            }
            if (name == "log") {
                if (argument <= 0.0) {
                    throw std::domain_error("log requires a positive argument");
                }
                return checked(std::log10(argument));
            }
            if (name == "sin") {
                return checked(std::sin(toRadians(argument)));
            }
            if (name == "cos") {
                return checked(std::cos(toRadians(argument)));
            }
            if (name == "tan") {
                const double reduced = m_degrees
                    ? std::remainder(argument, 180.0)
                    : std::remainder(argument,  pi());
                const double halfTurn = m_degrees ? 90.0 : pi() / 2.0;
                const double tolerance =
                    4.0 * std::numeric_limits<double>::epsilon() * halfTurn;
                if (std::fabs(std::fabs(reduced) - halfTurn) <= tolerance) {
                    throw std::domain_error("tan is undefined at odd multiples of 90 degrees");
                }
                return checked(std::tan(toRadians(argument)));
            }
            if (name == "asin") {
                return checked(fromRadians(std::asin(requireUnitInterval(argument, name))));
            }
            if (name == "acos") {
                return checked(fromRadians(std::acos(requireUnitInterval(argument, name))));
            }
            if (name == "atan") {
                return checked(fromRadians(std::atan(argument)));
            }
            if (name == "exp") {
                return checked(std::exp(argument));
            }
            if (name == "edx") {
                if (argument == 0.0) {
                    throw std::domain_error("Reciprocal of zero");
                }
                return checked(1.0 / argument);
            }

            throw std::invalid_argument("Unknown function: " + name);
        }

        static double requireUnitInterval(double value, const std::string& name)
        {
            if (value < -1.0 || value > 1.0) {
                throw std::domain_error(name + " argument must be between -1 and 1");
            }
            return value;
        }

        double toRadians(double value) const
        {
            // Reduce degrees before conversion to avoid overflow and retain
            // precision for large angles.
            return m_degrees ? std::remainder(value, 360.0) * pi() / 180.0 : value;
        }

        double fromRadians(double value) const
        {
            return m_degrees ? value * 180.0 / pi() : value;
        }

        static double pi()
        {
            return std::acos(-1.0);
        }
    };
};

#endif // UTILS_COMMON_H
