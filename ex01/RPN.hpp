
#pragma once
#include <stack>
#include <string>

class RPN
{
public:
    RPN(){}
    ~RPN(){}
    RPN(const RPN &other);
    RPN &operator=(const RPN &other);

    void calculate(const std::string &expression);
    
private:
    std::stack<double> _stack;          //store only numbers.
    std::string _operator_tokens;                //store input string.
    
    void processToken(const std::string &token);
    bool isOperator(const std::string &token) const;
    bool isNumber(const std::string &token) const;
    void operation(char op);
};