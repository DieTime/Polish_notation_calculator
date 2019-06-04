// Created by Denis Glazkov on 09.05.2019.

#ifndef CONFIG_H
#define CONFIG_H

#include <map>

std::string list_of_operators[]={
        "+", "-", "*", "/", "^", "(", ")","deg", "rad", "ln", "lg", "sin",
        "cos", "tan", "cotan", "asin", "acos","atan", "acot", "sqrt"};

std::string list_of_inverse_trigonometric_operators[] = {"asin", "acos","atan", "acot"};

std::map <std::string, int> priority{
    {"+", 1},  {"-", 1}, {"*", 2},  {"/", 2}, {"^", 3},
    {"sin", 4},  {"cos", 4},  {"tan", 4}, {"cotan", 4},
    {"asin", 4}, {"acos", 4}, {"atan", 4}, {"acot", 4},
    {"sqrt", 4}, {"ln", 4}, {"lg", 4}};

std::string constants[] = {"deg", "rad", "pi", "exp (", "exp", "e"};

std::map <std::string, std::string> replaceable_constants{
    {"deg", " / 180 * pi"}, {"rad",""}, {"pi", "3.141592653589793238462643"},
    {"exp (", "2.71828182845904523536 ^ ("}, {"exp", "2.71828182845904523536"},
    {"e", "2.71828182845904523536"}};

const float Pi = 3.141592653589793238462643f;
const int number_of_operators_for_parsing = 10;

#endif //CONFIG_H
