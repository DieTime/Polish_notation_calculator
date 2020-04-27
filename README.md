<p align="center">
  <img src="https://i.ibb.co/qxjZFYp/1.png" width="450">
</p>
<p align="center">
  <img src="https://img.shields.io/badge/version-1.0.0-green.svg?style=flat-square" alt="version">
</p>

# RPN-Calculator
Calculator using Reverse Polish notation

### Supports:

1) Operators: `+, -, *, /, ^, (, )`
2) Functions: `ln(x), lg(x), sin(x), cos(x), tan(x), cotan(x), asin(x), acos(x), atan(x), acot(x), sqrt(x), exp(x)`
3) `Work with deg and rad`
4) `Auto-formatting entered text`
5) Constants: `exp` and `pi`

### Example:
```cpp
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
```
