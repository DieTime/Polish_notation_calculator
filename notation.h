// Created by Denis Glazkov on 09.05.2019.

#ifndef NOTATION_H
#define NOTATION_H

#include <algorithm>
#include <vector>
#include <cmath>
#include "config.h"

//Convert one char symbol to string
std::string convert_to_string(char mychar)
{
    std::stringstream ss;
    std::string target;
    ss << mychar;
    ss >> target;
    return target;
}

//Is string a number
int is_digit(std::string str)
{
    /* If char from str isn't in ['1','2','3','4','5','6','7','8','9','0','.', '-']
       then str isn't a digit */
    std::string alphabet = "1234567890.-";
    for (char i : str) {
        if (alphabet.find(i) == std::string::npos)
            return 0;
    }
    return 1;
}

// Check if the string is an operator
int is_operator(const std::string &a)
{
    // Parsing the list of operators and looking for matches
    for (const std::string &i : list_of_operators)
    {
        if (a == i)
            return true;
    }
    return false;
}

// Delete extra spaces
void trim_str(std::string &str)
{
    int i = 0;

    while (i < (int)str.length() - 1)
    {
        if ((str[i] == ' ') && (str[i + 1] == ' '))
        {
            str.erase(static_cast<unsigned int>(i), 1);
            i--;
        }
        i++;
    }

    // Trim the remaining space to the left
    if (str[0] == ' ')
        str.erase(0, 1);

    // Trim the remaining space to the right
    if (str[str.length() - 1] == ' ')
        str.erase(str.length() - 1, 1);
}

// Returns a list of indexes of all substring inclusions in an expression.
std::vector<int> pos_in_expression(std::string str, std::string substr)
{
    std::vector<int> positions = {};

    int pos; // Negative position not possible for string
    int i = 0, j; // Counters

    //Parsing all symbols
    while (i < (int)str.length())
    {
        // If found begin character of operator
        if (str[i] == substr[0])
        {
            pos = i;
            j = 1;
            // Exception if minus is not an operator
            if ((is_operator(convert_to_string(str[i-1])) || (i==0)))
            {
                pos = -1;
            }
            i++;
            /* Make sure we found an operator
               (All operator characters are equal to characters in the string. */
            while (j < (int)substr.length())
            {
                if (str[i] != substr[j])
                {
                    pos = -1;
                    break;
                }
                i++;
                j++;
            }
            // If the position in the string is possible, add it in a list of positions
            if (pos != -1)
                positions.push_back(pos);
        }
        i++;
    }
    return positions;
}

// Modifies the expression to the correct form
void parse_expression(std::string &expression)
{
    std::vector<int> positions; // Set of positions of found operator

    // Variable for correct insertion of spaces, due to increasing line length
    int corrective = 0;

    // Translate a string to lower case
    transform(expression.begin(), expression.end(), expression.begin(), ::tolower);

    // Parse in incomplete list of operators { "+", "-", "*", "/", "^", "(", ")", "ln", "lg" }
    for (int i = 0; i <= 8; i++)
    {
        // Getting list of operator positions
        positions = pos_in_expression(expression, list_of_operators[i]);

        // If positions list isn't empty
        if (!positions.empty())
        {
            // Add spaces to the side of the operator
            corrective = 0;
            for (int j : positions)
            {
                expression.replace(static_cast<unsigned int>(j + corrective), list_of_operators[i].length(), " " + list_of_operators[i] + " ");
                corrective += 2;
            }
        }
    }

    // Replace all "pi" constants
    std::string::size_type n=0;
    while ((n = expression.find("pi", n)) != std::string::npos)
    {
        expression.replace(n, 2, "3.141592653589793238462643");
        n += 26;
    }

    n = 0;
    // Replace all exponent degrees
    while ((n = expression.find("exp (", n)) != std::string::npos)
    {
        expression.replace(n, 5, "2.71828182845904523536 ^ (");
        n += 26;
    }

    n = 0;
    // //Replace all "exp" constants
    while ((n = expression.find("exp", n)) != std::string::npos)
    {
        expression.replace(n, 3, "2.71828182845904523536");
        n += 22;
    }

    n = 0;
    //Replace all "e" constants
    while ((n = expression.find('e', n)) != std::string::npos)
    {
        expression.replace(n, 1, "2.71828182845904523536");
        n += 22;
    }

    trim_str(expression);
    std::cout << expression << std::endl;
}

// Returns the expression converted to Polish notation in the form of a list
std::vector<std::string> to_polish_notation(std::string &expression)
{
    int i = 0;
    std::string word; // String to parse the expression
    std::vector <std::string> result; // Result array of polish notation
    std::vector <std::string> operators; // Operators array of polish notation

    while (i < (int)expression.size()) // Until the end of the line
    {
        // Get the word
        while ((expression[i] != ' ') && (i < (int)expression.size()))
        {
            word.push_back(expression[i]);
            i++;
        }

        // If word isn't operator
        if (!is_operator(word))
        {
            result.push_back(word); // Add word to the Operators array
        }

            // If word is operator
        else
        {
            // If Operators array isn't empty
            if (!operators.empty())
            {
                // If word is a open bracket
                if (word == "(")
                {
                    operators.push_back(word);
                }
                    // If word is a close bracket
                else if (word == ")")
                {
                    /* Until we reach the opening bracket we move the operators
                       from the operator stack to the Result stack */
                    while (operators[operators.size() - 1] != "(")
                    {
                        result.push_back(operators[operators.size() - 1]);
                        operators.pop_back();
                        if (operators.empty())
                            break;
                    }
                    // Getting rid of brackets
                    operators.pop_back();
                }
                else
                {
                    /* We move all operators with higher priority to those
                    found in the Result stack */
                    while (priority[operators[operators.size() - 1]] >= priority[word])
                    {
                        result.push_back(operators[operators.size() - 1]);
                        operators.pop_back();
                        if (operators.empty())
                            break;
                    }
                    // Add operator to the Result stack
                    operators.push_back(word);
                }
            }
                // If Operators array is empty
            else
            {
                // Add operator to the Result stack
                operators.push_back(word);
            }
        }
        // Clearing the found word
        word.clear();
        i++;
    }
    /* Upon completion, move everything from the
    operator stack to the result stack */
    while ((int)operators.size() != 0)
    {
        result.push_back(operators[operators.size() - 1]);
        operators.pop_back();
    }
    return result;
}

// Gets an answer from the Polish notation if it's possible
std::string get_answer(std::vector<std::string> notation)
{
    std::vector <double> answer; // Result of calculations
    double temp; // Temp variable for calculating
    std::string result;

    //Parse polish notation
    for (const std::string &str : notation)
    {
        // If the founded string is not an operator, insert it into the list of answers
        if (!is_operator(str))
            answer.push_back(stof(str));

            /* If the founded string is an operator, perform the necessary
               actions with the elements of the answer list */
        else
        {
            if (str == "*")
            {
                if (answer.size() < 2)
                {
                    result = "error";
                    break;
                }
                temp = answer[answer.size() - 2] * answer[answer.size() - 1];
                answer.pop_back();
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "/")
            {
                if (answer.size() < 2)
                {
                    result = "error";
                    break;
                }
                if (answer[answer.size() - 1] == 0)
                {
                    result = "zero_exception";
                    break;
                }
                temp = answer[answer.size() - 2] / answer[answer.size() - 1];
                answer.pop_back();
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "+")
            {
                if (answer.size() < 2)
                {
                    result = "error";
                    break;
                }
                temp = answer[answer.size() - 2] + answer[answer.size() - 1];
                answer.pop_back();
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "-")
            {
                if (answer.size() < 2)
                {
                    result = "error";
                    break;
                }
                temp = answer[answer.size() - 2] - answer[answer.size() - 1];
                answer.pop_back();
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "^")
            {
                if (answer.size() < 2)
                {
                    result = "error";
                    break;
                }
                temp = pow(answer[answer.size() - 2], answer[answer.size() - 1]);
                answer.pop_back();
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "sin")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = std::sin(answer[answer.size() - 1] * Pi / 180);
                answer.pop_back();
                answer.push_back(static_cast<float &&>(temp));
            }
            if (str == "cos")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = std::cos(answer[answer.size() - 1] * Pi / 180);
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "tan")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = std::tan(answer[answer.size() - 1] * Pi / 180);
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "cotan")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = 1 / std::tan(answer[answer.size() - 1] * Pi / 180);
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "asin")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = std::asin(answer[answer.size() - 1]) * 180.0 / Pi;
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "acos")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = std::acos(answer[answer.size() - 1]) * 180.0 / Pi;
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "atan")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = std::atan(answer[answer.size() - 1] * Pi / 180);
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "acot")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = 1 / std::atan(answer[answer.size() - 1] * Pi / 180);
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "sqrt")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = std::sqrt(answer[answer.size() - 1]);
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "ln")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = std::log(answer[answer.size() - 1]);
                answer.pop_back();
                answer.push_back(temp);
            }
            if (str == "lg")
            {
                if (answer.empty())
                {
                    result = "error";
                    break;
                }
                temp = std::log10(answer[answer.size() - 1]);
                answer.pop_back();
                answer.push_back(temp);
            }
        }
    }
    if ((result != "error") && (result != "zero_exception"))
        result = std::to_string(answer[0]);
    // Return the answer that lies in 1 cell of the array
    return result;
}

//Brackets validation
int check_brackets(std::string expression)
{
    int count = 0; // Count of unclosed brackets

    /* If count of unclosed brackets became less than 0
       then brackets is  invalidation */
    for (char i : expression) {
        if (i == '(')
            count++;
        if (i == ')')
            count--;
        if (count < 0)
            return 0;
    }

    // If all brackets is closed then brackets is validation
    if (count == 0)
        return 1;
        //Else
    else
        return 0;
}

//Expression validation
int correct_expression(std::string &expression)
{
    int i = 0; // Counters
    std::string substr; // Word in expression

    //If the brackets are not placed correctly
    if (!check_brackets(expression))
        return 0;

    // Convert expression to correct form
    parse_expression(expression);

    /*If the word found is not a digit and
      the operator does not return 0 */
    while (i < expression.length())
    {
        substr.clear();
        while ((expression[i] != ' ') && (i < expression.length()))
        {
            substr.push_back(expression[i]);
            i++;
        }
        if ((!is_digit(substr)) && (!is_operator(substr)))
            return 0;
        if (i >= expression.length())
            break;
        i++;
    }
    //Else return 1
    return 1;
}

#endif //=NOTATION_H
