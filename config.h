// Created by Denis Glazkov on 09.05.2019.

#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <sstream>

std::string list_of_operators[] = { "+", "-", "*", "/", "^", "(", ")", "ln", "lg", "sin",
                                    "cos", "tan", "cotan", "asin", "acos",
                                    "atan", "acot", "sqrt"};

std::map <std::string, int> priority{ {"+", 1},  {"-", 1}, {"*", 2},  {"/", 2}, {"^", 3}, // Operations priority
                                      {"sin", 4},  {"cos", 4},  {"tan", 4}, {"cotan", 4},
                                      {"asin", 4}, {"acos", 4}, {"atan", 4}, {"acot", 4},
                                      {"sqrt", 4}, {"ln", 4}, {"lg", 4} };

const float Pi = 3.141592653589793238462643f;

#endif //CONFIG_H
