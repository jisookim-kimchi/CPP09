#include "rpn.hpp"
#include <iostream>
#include <sstream>

void RPN::processToken(const std::string &token)
{
    if (isOperator(token))
    {
        operation(token[0]);
    }
    else if (isNumber(token))
    {
        _stack.push(std::stod(token));
    }
    else
    {
        throw std::runtime_error("Error : invalid token");
    }
}

bool RPN::isOperator(const std::string &token) const
{
    if (token.size() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/'))
        return true;
    return false;
}

bool RPN::isNumber(const std::string &token) const
{
    if (token.size() == 1 && std::isdigit(token[0]))
        return true;
    return false;
}

void RPN::operation(char op)
{
    double val1, val2;

    if (_stack.size() < 2)
        throw std::runtime_error("Error : stack size is less than 2.");
    val2 = _stack.top();
    _stack.pop();
    val1 = _stack.top();
    _stack.pop();
    
    switch (op)
    {
        case '+':
            _stack.push(val1 + val2);
            break;
        case '-':
            _stack.push(val1 - val2);
            break;
        case '*':
            _stack.push(val1 * val2);
            break;
        case '/':
            if (val2 == 0)
                throw std::runtime_error("Error : try to divide by zero");
            _stack.push(val1 / val2);
            break;
    }
}

void RPN::calculate(const std::string &expression)
{
    std::istringstream iss(expression);
    std::string token;
    while (iss >> token)
    {
        processToken(token);
    }
    if (_stack.size() != 1)
        throw std::runtime_error("Error: invalid expression");
    std::cout << _stack.top() << std::endl;
}

RPN::RPN(const RPN &other)
{
    *this = other;
}

RPN &RPN::operator=(const RPN &other)
{
    _stack = other._stack;
    return *this;
}