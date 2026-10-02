#include "sort.hpp"
#include <climits>
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

/** 
    @brief : make pairs from vector.
    @param vpArr : src vector to make pairs.
    @param sadSingle : optional, single value if vector has odd number of elements.
    @return : vector of pairs.
    @TODO : make template function.
*/
std::vector<std::pair<int, int>> Sort::makePairs(const std::vector<int> &vpArr, std::optional<int> &sadSingle)
{
    std::vector<std::pair<int, int>> pairs;

    for(auto it = vpArr.begin(); it != vpArr.end() && (it + 1 != vpArr.end()); it += 2)
    {
        int first = *it;
        int second = *(it + 1);
        if (first > second)
            pairs.push_back({second, first}); // push {smaller, bigger}
        else
            pairs.push_back({first, second});
    }
    if (vpArr.size() % 2 != 0)
        sadSingle = vpArr.back();
    return pairs;
}

std::vector<int> Sort::mergeInsertionSort(const std::vector<int> &arr)
{
    if (arr.size() <= 1)
        return arr;

    std::optional<int> sadSingle;
    std::vector<std::pair<int, int>> pairs = makePairs(arr, sadSingle);
    std::vector<int> bigNums;
    for (const auto &p : pairs)
        bigNums.push_back(p.second);

    std::vector<int> sortedBigNumbers = mergeInsertionSort(bigNums);
    std::vector<int> storedSmallNumPairs;
    for (const auto &big : sortedBigNumbers)
    {
        for (auto it = pairs.begin(); it != pairs.end(); ++it)
        {
            if (it->second == big)
            {
                storedSmallNumPairs.push_back(it->first);
                pairs.erase(it);
                break;
            }
        }   
    }
    std::vector<int> mergedBigAndSmall = sortedBigNumbers;
    for (int small : storedSmallNumPairs)
    {
        mergedBigAndSmall.push_back(small);
    }
    if (sadSingle.has_value())
        mergedBigAndSmall.push_back(sadSingle.value());

    //TODO : small number insert in Jacobsthal sequence.
    return mergedBigAndSmall;
}


void Sort::printTest()
{
    std::cout << "Input : ";
    for (auto it = _vec.begin(); it != _vec.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl
    << std::endl;

    std::cout << "--- print mergeInsertionSort ---\n";
    std::vector<int> result = mergeInsertionSort(_vec);
    std::cout << "\nReturned sortedBigNumbers: ";
    for (int i : result)
        std::cout << i << " ";
    std::cout << "\n";
}
