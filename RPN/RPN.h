#ifndef POLISH_NOTATION_H
#define POLISH_NOTATION_H

#include <vector>
#include <string>
#include <map>

class RPN {
private:
    std::string expression;

    static bool isDigit(const std::string &str);
    bool isOperator(const std::string &str);
    bool isTrigo(const std::string &str);

    std::string toCorrectExpression(const std::string &expr);
    void validExpression(const std::string &expr);
    static bool checkBrackets(const std::string &expr);
    static std::string removeExtraSpaces(const std::string &str);

    std::vector<std::string> toPolish(const std::string &expr);
    std::string toResult(const std::vector<std::string> &Notation);

    std::vector<std::string> operators;
    std::vector<std::string> trigonometric;
    std::map <std::string, int> priority;
    std::map <std::string, std::string> constants;
public:
    RPN();
    explicit RPN(const std::string& expr);

    std::string getExpression();
    void setExpression(const std::string& expr);
    std::vector<std::string> getNotation();
    std::string solve();

    friend std::istream& operator >> (std::istream &in, RPN& pn);
    friend std::ostream& operator << (std::ostream &out, RPN& pn);
};

#endif //POLISH_NOTATION_H
