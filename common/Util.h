/*
 * Copyright (c) 2026 Koch, Achim Johannes Josef.
 * SPDX-License-Identifier: MIT
 */

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
/**
 * @brief Calculates the floating-point remainder of two values.
 * @param x Dividend.
 * @param y Divisor; zero causes std::domain_error.
 * @return The remainder as produced by std::fmod.
 */
static double sie_mod(double x, double y)
{
    if (y == 0.0) {
        throw std::domain_error("Modulo by zero");
    }
    return std::fmod(x, y);
}

/**
 * @brief Calculates the factorial of a non-negative integer.
 * @param n Integer whose factorial is calculated.
 * @return n! as a double (0! and 1! are 1).
 * @throws std::domain_error if n is negative.
 * @throws std::overflow_error if the result is not finite.
 * @note Kept as a compatibility helper; the implementation is iterative.
 */
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

/**
 * @brief Tests whether two floating-point values differ by at most epsilon.
 * @param a First value.
 * @param b Second value.
 * @param epsilon Maximum accepted absolute difference (default: 1e-9).
 * @return true if |a - b| <= epsilon; otherwise false.
 */
static bool compareDoubles(double a, double b, double epsilon = 1e-9)
{
    return std::fabs(a - b) <= epsilon;
}

/**
 * A small recursive-descent expression evaluator.
 *
 * Supported operators:
 *   + - * /       arithmetic
 *   mod           floating-point remainder\n *   root          x-th root (y root x means x-th root of y)
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
    /**
     * @brief Parses and evaluates a mathematical expression.
     * @param expression Expression text in the calculator's supported syntax.
     * @return Evaluated result, converted to long double for the public API.
     * @throws std::invalid_argument for empty, malformed or unknown expressions.
     * @throws std::domain_error for undefined mathematical operations.
     * @throws std::overflow_error when an intermediate or final result overflows.
     * @throws std::length_error when the parser nesting limit is exceeded.
     */
    long double evaluate(const std::string& expression) const
    {
        Parser parser(expression, m_degrees, m_inverted);
        return parser.parse();
    }

    /** @brief Selects the angle unit used by trigonometric functions.
     * @param isDegrees true for degrees (default), false for radians.
     */
    void setDegMode(bool isDegrees)
    {
        m_degrees = isDegrees;
    }

    /** @brief Enables or disables inverse calculator-key behavior.
     * @param isInverted true enables inverse percentage calculation.
     */
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

        /** @brief Evaluates the complete expression and rejects trailing input.
         * @return The calculated value.
         * @throws std::invalid_argument if the expression is empty or contains trailing/invalid input.
         */
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

        static constexpr std::size_t maxRecursionDepth = 256;
        std::size_t m_recursionDepth = 0;

        /**
         * @brief RAII guard that bounds recursive parser calls.
         * Increments the shared depth on construction and decrements it on destruction,
         * including when parsing exits by throwing an exception.
         * @throws std::length_error if maxRecursionDepth would be exceeded.
         */
        class RecursionGuard
        {
        public:
            explicit RecursionGuard(std::size_t& depth) : m_depth(depth)
            {
                if (++m_depth > maxRecursionDepth) {
                    --m_depth;
                    throw std::length_error("Expression nesting limit exceeded");
                }
            }

            ~RecursionGuard()
            {
                --m_depth;
            }

            RecursionGuard(const RecursionGuard&) = delete;
            RecursionGuard& operator=(const RecursionGuard&) = delete;

        private:
            std::size_t& m_depth;
        };

        /** @brief Returns whether the parser reached the end of the expression. */
        bool atEnd() const
        {
            return m_position >= m_expression.size();
        }

        /** @brief Returns the current character, or '\0' at end of input. */
        char current() const
        {
            return atEnd() ? '\0' : m_expression[m_position];
        }

        /** @brief Advances the input position over whitespace characters. */
        void skipWhitespace()
        {
            while (!atEnd() &&
                   std::isspace(static_cast<unsigned char>(current()))) {
                ++m_position;
            }
        }

        /** @brief Consumes the expected character after optional whitespace.
         * @return true if the character was present and consumed.
         */
        bool consume(char expected)
        {
            skipWhitespace();
            if (current() != expected) {
                return false;
            }
            ++m_position;
            return true;
        }

        /** @brief Consumes a keyword, requiring an alphabetic word boundary.
         * @param word Keyword to match.
         * @return true if the keyword was present and consumed.
         */
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

        /** @brief Validates a computed result before it is returned or reused.
         * @throws std::overflow_error for infinity and std::domain_error for NaN.
         */
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

        /** @brief Parses left-associative addition and subtraction. */
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

        /** @brief Parses multiplication, division, modulo and percentage operations.
         * @throws std::domain_error for division or modulo by zero.
         */
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
                } else if (consumeWord("nCr")) {
                    const double k = parseUnary();
                    left = checked(binomialCoefficient(left, k));
                } else if (consumeWord("root")) {
                    // "y root x" means the x-th root of y.
                    const double index = parseUnary();
                    if (index == 0.0) {
                        throw std::domain_error("Root index is zero");
                    }
                    if (left < 0.0) {
                        if (std::floor(index) != index ||
                            std::fmod(std::fabs(index), 2.0) != 1.0) {
                            throw std::domain_error(
                                "Even root of a negative number is undefined");
                        }
                        left = checked(-std::pow(-left, 1.0 / index));
                    } else {
                        left = checked(std::pow(left, 1.0 / index));
                    }
                } else {
                    return left;
                }
            }
        }

        // Unary signs bind less tightly than exponentiation:
        // -2^2 is -(2^2), while 2^-2 is valid.
        /** @brief Parses unary signs, which bind less tightly than exponentiation.
         * @throws std::length_error if recursive expression nesting is excessive.
         */
        double parseUnary()
        {
            RecursionGuard recursionGuard(m_recursionDepth);
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
        /** @brief Parses exponentiation with right associativity and signed exponents. */
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

        /** @brief Parses postfix factorial operators.
         * @throws std::domain_error unless the operand is a non-negative integer.
         * @throws std::overflow_error when the factorial exceeds the supported range.
         */
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

        /** @brief Parses a number, pi, parenthesized expression or function call.
         * @throws std::length_error if recursive nesting exceeds the parser limit.
         */
        double parsePrimary()
        {
            RecursionGuard recursionGuard(m_recursionDepth);
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
            if (consumeWord("e")) {
                return std::exp(1.0);
            }

            if (std::isalpha(static_cast<unsigned char>(current()))) {
                return parseFunction();
            }

            throw std::invalid_argument(
                "Invalid character: " + std::string(1, current()));
        }

        /** @brief Parses and evaluates a named single-argument mathematical function.
         * @throws std::invalid_argument for unknown functions or malformed calls.
         * @throws std::domain_error when the function argument is outside its domain.
         */
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

        /** @brief Calculates the binomial coefficient n choose k.
         * @throws std::domain_error unless n and k are non-negative integers with k <= n.
         * @throws std::length_error when the calculation would require too many iterations.
         * @throws std::overflow_error when the result exceeds the supported numeric range.
         */
        static double binomialCoefficient(double n, double k)
        {
            if (n < 0.0 || k < 0.0 ||
                std::floor(n) != n || std::floor(k) != k) {
                throw std::domain_error("nCr requires non-negative integers");
            }
            if (k > n) {
                throw std::domain_error("nCr requires k <= n");
            }

            // Above 2^53-1, double can no longer represent every integer exactly.
            constexpr double maxExactInteger = 9007199254740991.0;
            if (n > maxExactInteger) {
                throw std::overflow_error("nCr argument is out of range");
            }

            // Use symmetry and the multiplicative formula instead of factorials:
            // this avoids overflowing intermediate factorials for valid results.
            const double iterations = k < n - k ? k : n - k;
            if (iterations > 100000.0) {
                throw std::length_error("nCr calculation exceeds supported limit");
            }

            double result = 1.0;
            const int count = static_cast<int>(iterations);
            for (int i = 1; i <= count; ++i) {
                result *= (n - iterations + static_cast<double>(i)) /
                          static_cast<double>(i);
                result = checked(result);
            }
            return result;
        }

        /** @brief Validates an inverse-trigonometric argument in the interval [-1, 1].
         * @param value Argument to validate.
         * @param name Function name used in the diagnostic message.
         * @return The unchanged argument.
         * @throws std::domain_error if value is outside [-1, 1].
         */
        static double requireUnitInterval(double value, const std::string& name)
        {
            if (value < -1.0 || value > 1.0) {
                throw std::domain_error(name + " argument must be between -1 and 1");
            }
            return value;
        }

        /** @brief Converts an angle to radians when degree mode is active.
         * @param value Angle in the currently selected input unit.
         * @return Angle in radians; degree values are reduced modulo 360 first.
         */
        double toRadians(double value) const
        {
            // Reduce degrees before conversion to avoid overflow and retain
            // precision for large angles.
            return m_degrees ? std::remainder(value, 360.0) * pi() / 180.0 : value;
        }

        /** @brief Converts radians to degrees when degree mode is active.
         * @param value Angle in radians.
         * @return Angle in the currently selected output unit.
         */
        double fromRadians(double value) const
        {
            return m_degrees ? value * 180.0 / pi() : value;
        }

        /** @brief Returns pi, calculated from acos(-1). */
        static double pi()
        {
            return std::acos(-1.0);
        }
    };
};

#endif // UTILS_COMMON_H
