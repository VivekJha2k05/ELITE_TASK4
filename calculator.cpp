#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

class Parser {
    string expr;
    size_t pos = 0;

public:
    Parser(const string& input) {
        // Remove spaces automatically
        for (char c : input) {
            if (!isspace(c)) expr += c;
        }
    }

    double parse() {
        double result = expression();
        if (pos < expr.size()) {
            throw runtime_error("Unexpected character: " + string(1, expr[pos]));
        }
        return result;
    }

private:
    double expression() {
        double result = term();
        while (pos < expr.size()) {
            if (expr[pos] == '+') {
                ++pos;
                result += term();
            } else if (expr[pos] == '-') {
                ++pos;
                result -= term();
            } else {
                break;
            }
        }
        return result;
    }

    double term() {
        double result = factor();
        while (pos < expr.size()) {
            if (expr[pos] == '*') {
                ++pos;
                result *= factor();
            } else if (expr[pos] == '/') {
                ++pos;
                double divisor = factor();
                if (divisor == 0) throw runtime_error("Division by zero!");
                result /= divisor;
            } else {
                break;
            }
        }
        return result;
    }

    double factor() {
        if (pos < expr.size() && expr[pos] == '(') {
            ++pos;
            double result = expression();
            if (pos >= expr.size() || expr[pos] != ')') throw runtime_error("Expected ')'");
            ++pos;
            return result;
        }
        return number();
    }

    double number() {
        size_t start = pos;
        while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.')) ++pos;
        if (start == pos) throw runtime_error("Expected number");
        return stod(expr.substr(start, pos - start));
    }
};

int main() {
    cout << "Enter an arithmetic expression (e.g., 2 + 3 * (4 - 1)): ";
    string input;
    getline(cin, input);

    try {
        Parser parser(input);
        double result = parser.parse();
        cout << "Result: " << result << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}