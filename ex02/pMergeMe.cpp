#include "pMergeMe.hpp"
#include <climits>
#include <algorithm>
/**
    @brief : get int from string.
    @param str : argv[1], input value from Program argument to sort.
*/
int pMergeMe::fromChartoInt(const std::string &str)
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
            throw std::invalid_argument("Error: str is not positive integer");
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

pMergeMe::pMergeMe(int argc, char *argv[]) : _isSwapped(false)
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

void pMergeMe::printResult()
{
    std::cout << "Before: ";
    for (int num : _vec)
        std::cout << num << " ";
    std::cout << std::endl;

    auto [resultVec, vecTime] = measureSort(_vec);
    auto [resultDeq, deqTime] = measureSort(_deq);

    std::cout << "After:  ";
    for (int num : resultVec)
        std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "Time to process a range of " << _vec.size()
              << " elements with std::vector : " << std::fixed << std::setprecision(5) << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _deq.size()
              << " elements with std::deque : " << std::fixed << std::setprecision(5) << deqTime << " us" << std::endl;
}

