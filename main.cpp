// Created by Denis Glazkov on 09.05.2019.

#include <iostream>
#include "notation.h"

using namespace std;

int main()
{
    vector <string> notation; // Polish notation
    string expression; // Input expression
    string answer; // Result of calculations

    while (true)
    {
        // Read the expression until you press enter
        cout << "Enter expression >> ";
        getline(cin, expression);
        if (expression.empty())
            break;

        //Check expression for correctness
        if (!correct_expression(expression))
        {
            cout << "Incorrect expression\n" << endl;
            continue;
        }

        // Convert expression to polish notation
        notation = to_polish_notation(expression);

        // Getting the answer if function doesn't return "error" or "zero_exception"
        answer = get_answer(notation);

        //If an error occurred while evaluating the expression
        if (answer == "error")
        {
            cout << "Incorrect expression\n" << endl;
            continue;
        }

        //If the calculation appeared division by zero
        if (answer == "zero_exception")
        {
            cout << "Division by zero exception\n" << endl;
            continue;
        }

        // Display the result on the screen
        cout << answer << endl << endl;
    }

    return 0;
}
