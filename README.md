## **RPN-Calculator**
String calculator using reverse polish notation

#### Supports:

1) Operators: `+, -, *, /, ^, (, )`
2) Functions: `ln(x), lg(x), sin(x), cos(x), tan(x), cotan(x), asin(x), acos(x), atan(x), acot(x), sqrt(x), exp(x)`
3) `Work with deg and rad` // example sin(30 deg)
4) `Auto-formatting of the entered text` (works without extra spaces between operators ,functions and constants)
5) Constants: exp and pi

#### Main functions:

1) `int correct_expression(std::string &expression)` //Expression validation and changes the expression to the correct form
2) `void parse_expression(std::string &expression)` //Modifies the expression to the correct form
3) `vector <std::string> to_polish_notation(std::string &expression)` // Returns the RPN as a list. Only works with parsed expressions(after the parse_expression function)
4) `string get_answer(std::vector<std::string> notation)` //Gets an answer from the Polish notation if it's possible

#### Enjoy😉
