#include "rpn.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    RPN rpn;
    
    try
    {
        if (argc != 2)
            throw std::runtime_error("Usage: ./rpn [RPN expression]");
        rpn.calculate(argv[1]);
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;    
}