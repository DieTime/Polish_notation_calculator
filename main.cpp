#include <iostream>
#include "RPN/RPN.h"

using namespace std;

int main() {
    RPN expr("sin(30 deg) + 5^2 - ln(10)");

    // Get expression in reverse polish notation
    vector<string> notation = expr.getNotation();

    // Get expression and solution
    cout << "Solution of " << expr.getExpression() << " is " << expr.solve() << endl;

    cin >> expr; // Set expression
    cout << expr; // Get solution

    return 0;
}
