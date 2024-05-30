#ifndef UTILS_COMMON_H
#define UTILS_COMMON_H

// put common code here

#include <string>
#include <stdexcept>
#include <cmath>
#include <QString>

/**/
static double sie_mod(double x, double y)
{
    if (y == 0) return x;
    long i = (long)(x / y);
    if ( (x*y) < 0) --i;
    x = x - ((double)i) * y;
    if (x == y) x -= y;
    return x;
}


static bool compareDoubles(double a, double b, double epsilon = 1e-9) {
    return fabs(a - b) < epsilon;
}

static bool m_isGrad = true;

class Calculator {

public:
    static double evaluate(const std::string& expression) {
        size_t pos = 0;
        return evaluateExpression(expression, pos);
    }

    static void setDegMode(bool isGrad)
    {
        m_isGrad = isGrad;
    }

private:
    static double evaluateExpression(const std::string& expression, size_t& pos) {
        double result = parseAdditionSubtraction(expression, pos);
        skipWhitespace(expression, pos);
        return result;
    }

    static double converGradtoRad(double arg)
    {
        return arg * M_PI / 180.0;
    }

    static double convertRadtoGrad(double arg)
    {
        return arg / M_PI * 180;
    }

    static double convertToRadIfGrad(double arg)
    {
        return m_isGrad ? converGradtoRad(arg) : arg;
    }

    static double convertToGradifGrad(double arg)
    {
        return m_isGrad ? convertRadtoGrad(arg) : arg;
    }

    static double parseAdditionSubtraction(const std::string& expression, size_t& pos) {
        double left = parseMultiplicationDivision(expression, pos);
        skipWhitespace(expression, pos);

        if(std::isalpha(expression[pos]))
        {
            std::string opName;
            while (pos < expression.size() && std::isalpha(expression[pos])) {
                opName += expression[pos++];
            }

            if(opName == "mod")
            {
                if (expression[pos] != ' ') {
                    throw std::invalid_argument("Expected ' ' after mod");
                }

                double right = parseMultiplicationDivision(expression, pos);
                skipWhitespace(expression, pos);

                left = sie_mod(left, right);

                return left;
            }
        }

        while (pos < expression.size() && (expression[pos] == '+' || expression[pos] == '-')) {
            char op = expression[pos];
            ++pos;

            double right = parseMultiplicationDivision(expression, pos);
            skipWhitespace(expression, pos);

            if (op == '+') {
                left += right;
            }
            else {
                left -= right;
            }
        }

        return left;
    }

    static double parseMultiplicationDivision(const std::string& expression, size_t& pos) {
        double left = parseExponentiation(expression, pos);
        skipWhitespace(expression, pos);

        while (pos < expression.size() && (expression[pos] == '*' || expression[pos] == '/'))
        {
            char op = expression[pos];

            ++pos;

            double right = parseExponentiation(expression, pos);
            skipWhitespace(expression, pos);

            if (op == '*') {
                left *= right;
            }
            else if (op == '/'){
                if (right == 0) {
                    throw std::invalid_argument("Division by zero");
                }
                left /= right;
            }
        }

        return left;
    }

    static double parseExponentiation(const std::string& expression, size_t& pos) {
        double base = parseFunctionOrAtom(expression, pos);
        skipWhitespace(expression, pos);

        if (pos < expression.size() && expression[pos] == '^') {
            ++pos;
            double exponent = parseExponentiation(expression, pos);
            skipWhitespace(expression, pos);
            return std::pow(base, exponent);
        }

        return base;
    }

    static double parseFunctionOrAtom(const std::string& expression, size_t& pos) {
        skipWhitespace(expression, pos);

        if (pos >= expression.size()) {
            throw std::invalid_argument("Unexpected end of expression");
        }

        char ch = expression[pos];

        if (ch == '(') {
            ++pos;
            double value = evaluateExpression(expression, pos);
            skipWhitespace(expression, pos);
            if (pos >= expression.size() || expression[pos] != ')') {
                throw std::invalid_argument("Mismatched parenthesis");
            }
            ++pos;
            return value;
        }
        // check for a constant with an multiplikator in front like 2pi
        if( (pos+2 < expression.size()))
        {
            int factor = 1;
            if( std::isdigit(ch) && expression.substr(pos+1, 2) == "pi" )
            {
                size_t endPos;
                factor = std::stoi(expression.substr(pos), &endPos);
                pos += 3;
                return M_PI * factor;
            }
        }
        // This is pi without an factor in front.
        if (pos+1 < expression.size() && std::isalpha(ch))
        {
            if(expression.substr(pos, 2) == "pi")
            {
                pos += 2;
                return M_PI;
            }
        }
        if ( std::isdigit(ch) || ch == '.' || ch == '-') {
                size_t endPos;
                double value = std::stod(expression.substr(pos), &endPos);
                pos += endPos;
                return value;
        }
        else if (std::isalpha(ch)) {
            std::string funcName = "";
            while (pos < expression.size() && std::isalpha(expression[pos])) {
                funcName += expression[pos++];
            }

            if (funcName == "sqrt") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after sqrt");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in sqrt");
                }
                ++pos;
                return std::sqrt(arg);
            }
            else if (funcName == "ln") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after ln");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in ln");
                }
                ++pos;
                return std::log(arg);
            }
            else if (funcName == "log") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after log");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in log");
                }
                ++pos;
                return std::log10(arg);
            }
            else if (funcName == "sin") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after sin");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                arg = convertToRadIfGrad(arg); // Convert degrees to radians
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in sin");
                }
                ++pos;
                return std::sin(arg);
            }
            else if (funcName == "cos") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after cos");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                arg = convertToRadIfGrad(arg); // Convert degrees to radians
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in cos");
                }
                ++pos;
                return std::cos(arg);
            }
            else if (funcName == "tan") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after tan");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                arg = convertToRadIfGrad(arg); // Convert degrees to radians
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in tan");
                }
                ++pos;
                return std::tan(arg);
            }
            else if (funcName == "asin") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after sin");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in sin");
                }
                ++pos;
                return convertToGradifGrad( std::asin(arg) );
            }
            else if (funcName == "acos") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after cos");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in cos");
                }
                ++pos;
                return convertToGradifGrad( std::acos(arg) );
            }
            else if (funcName == "atan") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after tan");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in tan");
                }
                ++pos;
                return convertToGradifGrad( std::atan(arg) );
            }
            else if (funcName == "exp") {
                if (expression[pos] != '(') {
                    throw std::invalid_argument("Expected '(' after exp");
                }
                ++pos;
                double arg = evaluateExpression(expression, pos);
                skipWhitespace(expression, pos);
                if (pos >= expression.size() || expression[pos] != ')') {
                    throw std::invalid_argument("Mismatched parenthesis in exp");
                }
                ++pos;
                return std::exp(arg);
            }
            else {
                throw std::invalid_argument("Unknown function: " + funcName);
            }
        }

        else {
            throw std::invalid_argument("Invalid character: " + std::string(1, ch));
        }
    }

    static void skipWhitespace(const std::string& expression, size_t& pos) {
        while (pos < expression.size() && std::isspace(expression[pos])) {
            ++pos;
        }
    }
};


#endif

