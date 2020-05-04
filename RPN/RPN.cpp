#include <algorithm>
#include <cmath>
#include <sstream>

#include "RPN.h"

bool RPN::isDigit(const std::string &str) {
    if (str[0] == '-') return str.find_first_not_of("1234567890.", 1) == std::string::npos;
    else return str.find_first_not_of("1234567890.", 0) == std::string::npos;
}

bool RPN::isOperator(const std::string &str) {
    return std::find(operators.begin(), operators.end(), str) != operators.end();
}

bool RPN::isTrigo(const std::string &str) {
    return std::find(trigonometric.begin(), trigonometric.end(), str) != trigonometric.end();
}

bool RPN::checkBrackets(const std::string &expr) {
    int brackets = 0;
    for (const char &i : expr) {
        if (i == '(') brackets++;
        if (i == ')') brackets--;
        if (brackets < 0) return false;
    }
    return (brackets == 0);
}

void RPN::validExpression(const std::string &expr) {
    if (!checkBrackets(expr)) {
        throw std::runtime_error("Error in brackets");
    }

    std::istringstream ss(expr);
    while (ss) {
        std::string word;
        ss >> word;
        if (word.empty()) break;
        if (!isOperator(word) && !isDigit(word)) {
            throw std::runtime_error("There are invalid characters");
        }
    }
}

std::string RPN::removeExtraSpaces(const std::string &str) {
    std::string exprWithoutExtra = str;
    size_t pos = exprWithoutExtra.find("  ", 0);
    while (pos != std::string::npos) {
        exprWithoutExtra.erase(pos, 1);
        pos = exprWithoutExtra.find("  ", pos);
    }

    if (str.front() == ' ') exprWithoutExtra.erase(0, 1);
    if (str.back() == ' ') exprWithoutExtra.pop_back();

    return exprWithoutExtra;
}

std::string RPN::toCorrectExpression(const std::string &expr) {
    std::string correctExpr = expr;
    transform(correctExpr.begin(), correctExpr.end(), correctExpr.begin(), ::tolower);

    // Add spaces around this operators { "+", "*", "/", "^", "(", ")","deg", "rad", "ln", "lg", "-" }
    for (int i = 0; i <= 10; i++) {
        size_t pos = correctExpr.find(operators[i], 0);
        while (pos != std::string::npos) {
            correctExpr.replace(pos, operators[i].length(), " " + operators[i] + " ");
            pos = correctExpr.find(operators[i], pos + 2 + operators[i].length());
        }
    }

    // Replace all constants
    for (const auto &i : constants) {
        size_t pos = correctExpr.find(i.first, 0);
        while (pos != std::string::npos) {
            correctExpr.replace(pos, i.first.size(), i.second);
            pos = correctExpr.find(i.first, pos + i.second.length());
        }
    }

    return removeExtraSpaces(correctExpr);
}

std::vector<std::string> RPN::toPolish(const std::string& expr) {
    validExpression(expr);

    std::vector<std::string> polish; // Result array
    std::vector<std::string> op; // Operators array
    std::string last; // Last word
    bool fixDigit = false; // Next item is digit with "-"

    std::istringstream ss(expr);
    while (ss) {
        std::string word;
        ss >> word;
        if (word.empty()) break;

        // Since there is a space on the sides of each "-",
        // we find out whether it is a number or not
        if (word == "-" && (last.empty() || isOperator(last))) {
            fixDigit = true;
            continue;
        }

        if (fixDigit && isDigit(word)) {
            word.insert(0, "-");
            fixDigit = false;
        }

        if (!isOperator(word)) polish.push_back(word);
        else {
            if (!op.empty()) {
                if (word == "(") op.push_back(word);
                else if (word == ")") {
                    while (op.back() != "(" && !op.empty()) {
                        polish.push_back(op.back());
                        op.pop_back();
                    }
                    op.pop_back();
                } else {
                    while (!op.empty() && priority[op.back()] >= priority[word]) {
                        polish.push_back(op.back());
                        op.pop_back();
                    }
                    op.push_back(word);
                }
            } else op.push_back(word);
        }

        last = word;
    }

    polish.insert(polish.end(), op.rbegin(), op.rend());
    return polish;
}

std::string RPN::toResult(const std::vector<std::string> &Notation) {
    std::vector<double> answer;

    for (const std::string &str : Notation) {
        if (!isOperator(str))
            answer.push_back(stof(str));
        else {
            if (str == "*") {
                if (answer.size() < 2) {
                    throw std::runtime_error("Not enough operands for the * operator");
                }
                answer.push_back(answer[answer.size() - 2] * answer.back());
                answer.erase(answer.end() - 3, answer.end() - 1);
            }
            if (str == "/") {
                if (answer.size() < 2) {
                    throw std::runtime_error("Not enough operands for the / operator");
                }
                if (answer[answer.size() - 1] == 0) {
                    throw std::runtime_error("Division by zero");
                }
                answer.push_back(answer[answer.size() - 2] / answer.back());
                answer.erase(answer.end() - 3, answer.end() - 1);
            }
            if (str == "+") {
                if (answer.size() < 2) {
                    throw std::runtime_error("Not enough operands for the + operator");
                }
                answer.push_back(answer[answer.size() - 2] + answer.back());
                answer.erase(answer.end() - 3, answer.end() - 1);
            }
            if (str == "-") {
                if (answer.size() < 2) {
                    throw std::runtime_error("Not enough operands for the - operator");
                }
                answer.push_back(answer[answer.size() - 2] - answer.back());
                answer.erase(answer.end() - 3, answer.end() - 1);
            }
            if (str == "^") {
                if (answer.size() < 2) {
                    throw std::runtime_error("Not enough operands for the ^ operator");
                }
                answer.push_back(pow(answer[answer.size() - 2], answer[answer.size() - 1]));
                answer.erase(answer.end() - 3, answer.end() - 1);
            }
            if (str == "sin") {
                if (answer.empty()) {
                    throw std::runtime_error("sin has no parameter");
                }
                answer.push_back(std::sin(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "cos") {
                if (answer.empty()) {
                    throw std::runtime_error("cos has no parameter");
                }
                answer.push_back(std::cos(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "tan") {
                if (answer.empty()) {
                    throw std::runtime_error("tan has no parameter");
                }
                answer.push_back(std::tan(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "cotan") {
                if (answer.empty()) {
                    throw std::runtime_error("cotan has no parameter");
                }
                answer.push_back(1 / std::tan(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "asin") {
                if (answer.empty()) {
                    throw std::runtime_error("asin has no parameter");
                }
                answer.push_back(std::asin(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "acos") {
                if (answer.empty()) {
                    throw std::runtime_error("acos has no parameter");
                }
                answer.push_back(std::acos(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "atan") {
                if (answer.empty()) {
                    throw std::runtime_error("atan has no parameter");
                }
                answer.push_back(std::atan(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "acot") {
                if (answer.empty()) {
                    throw std::runtime_error("acot has no parameter");
                }
                answer.push_back(1 / std::atan(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "sqrt") {
                if (answer.empty()) {
                    throw std::runtime_error("sqrt has no parameter");
                }
                answer.push_back(std::sqrt(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "ln") {
                if (answer.empty()) {
                    throw std::runtime_error("ln has no parameter");
                }
                answer.push_back(std::log(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
            if (str == "lg") {
                if (answer.empty()) {
                    throw std::runtime_error("lg has no parameter");
                }
                answer.push_back(std::log10(answer[answer.size() - 1]));
                answer.erase(answer.end() - 2);
            }
        }
    }

    if (std::isnan(answer[0])) return "NaN";

    std::stringstream ss;
    ss << answer[0];
    return ss.str() + ((isTrigo(Notation.back())) ? " rad" : "");
}

RPN::RPN(const std::string &expr) {
    operators = {
            "+", "*", "/", "^", "(", ")", "deg", "rad", "ln", "lg", "-", "sin",
            "cos", "tan", "cotan", "asin", "acos", "atan", "acot", "sqrt"
    };

    trigonometric = {"asin", "acos", "atan", "acot"};

    priority = {
            {"+",     1},
            {"-",     1},
            {"*",     2},
            {"/",     2},
            {"^",     3},
            {"sin",   4},
            {"cos",   4},
            {"tan",   4},
            {"cotan", 4},
            {"asin",  4},
            {"acos",  4},
            {"atan",  4},
            {"acot",  4},
            {"sqrt",  4},
            {"ln",    4},
            {"lg",    4}};

    constants = {
            {"deg",   " / 180 * pi"},
            {"rad",   ""},
            {"pi",    "3.14159265358979323846"},
            {"exp (", "2.71828182845904523536 ^ ("},
            {"exp",   "2.71828182845904523536"},
            {"e",     "2.71828182845904523536"}
    };
    expression = expr;
}


RPN::RPN() {
    operators = {
            "+", "*", "/", "^", "(", ")", "deg", "rad", "ln", "lg", "-", "sin",
            "cos", "tan", "cotan", "asin", "acos", "atan", "acot", "sqrt"
    };

    trigonometric = {"asin", "acos", "atan", "acot"};

    priority = {
            {"+",     1},
            {"-",     1},
            {"*",     2},
            {"/",     2},
            {"^",     3},
            {"sin",   4},
            {"cos",   4},
            {"tan",   4},
            {"cotan", 4},
            {"asin",  4},
            {"acos",  4},
            {"atan",  4},
            {"acot",  4},
            {"sqrt",  4},
            {"ln",    4},
            {"lg",    4}};

    constants = {
            {"deg",   " / 180 * pi"},
            {"rad",   ""},
            {"pi",    "3.14159265358979323846"},
            {"exp (", "2.71828182845904523536 ^ ("},
            {"exp",   "2.71828182845904523536"},
            {"e",     "2.71828182845904523536"}
    };
}

std::string RPN::getExpression() {
    return expression;
}

void RPN::setExpression(const std::string& expr) {
    expression = expr;
}

std::vector<std::string> RPN::getNotation() {
    if (!expression.empty()) {
        return toPolish(toCorrectExpression(expression));
    } else {
        throw std::runtime_error("The expression is not correct");
    }
}

std::string RPN::solve() {
    return toResult(getNotation());
}

std::istream &operator>>(std::istream &in, RPN &pn) {
    std::string res;
    in >> res;
    pn.expression = res;
    return in;
}

std::ostream &operator<<(std::ostream &out, RPN &pn) {
    out << pn.solve();
    return out;
}