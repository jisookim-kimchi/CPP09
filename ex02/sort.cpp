#include "sort.hpp"
#include <climits>
#include <algorithm>
/**
    @brief : get int from string.
    @param str : argv[1], input value from Program argument to sort.
*/
int Sort::fromChartoInt(const std::string &str)
{
    if (str.empty())
    {
        throw std::invalid_argument("Error: str is empty");
    }
    long long val = 0;
    size_t i = 0;
    while (i < str.size())
    {
        if (str[i] < '0' || str[i] > '9')
        {
            throw std::invalid_argument("Error: str is not digit");
        }
        val = val * 10 + (str[i] - '0');
        if (val > INT_MAX)
        {
            throw std::out_of_range("Error: val is out of range");
        }
        i++;
    }
    return static_cast<int>(val);
}

Sort::Sort(int argc, char *argv[]) : _isSwapped(false)
{
    if (argc < 2)
        throw std::invalid_argument("Error: input is empty");
    for (int i = 1; i < argc; ++i)
    {
        int num = fromChartoInt(argv[i]);
        if (num <= 0)
            throw std::invalid_argument("Error: not a positive integer");
        _vec.push_back(num);
        _deq.push_back(num);
    }
}

void Sort::printTest()
{
    std::cout << "Input (vector) : ";
    for (auto it = _vec.begin(); it != _vec.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl;

    std::vector<int> resultVec = mergeInsertionSort(_vec);
    std::cout << "Sorted (vector): ";
    for (int i : resultVec)
        std::cout << i << " ";
    std::cout << "\n\n";

    std::cout << "Input (deque)  : ";
    for (auto it = _deq.begin(); it != _deq.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl;

    std::deque<int> resultDeq = mergeInsertionSort(_deq);
    std::cout << "Sorted (deque) : ";
    for (int i : resultDeq)
        std::cout << i << " ";
    std::cout << "\n";
}
