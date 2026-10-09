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
            pairs.push_back({second, first}); // push {first(smaller), second(bigger)}
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
    std::vector<std::pair<int, int>> pairs = makePairs(arr, sadSingle); // 2 6 7 3 9 8 4 7 10 -> [ 2,  6 ], [ 7,  3 ], [ 9,  8 ], [ 4,  7 ],  10
    std::vector<int> bigNums;
    for (const auto &p : pairs)
    {
        //std::cout << "pairs: " << p.first << " " << p.second << std::endl;
        bigNums.push_back(p.second);
        //std::cout << "---------------------------------------------" << std::endl;
    }

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
    std::cout << "\n================ [recursive sort] ================\n";
    std::cout << "sortedBigNumbers: ";
    for (int a : sortedBigNumbers)
        std::cout << a << " ";
    std::cout << "\nstoredSmallNumPairs: ";
    for (int b : storedSmallNumPairs)
        std::cout << b << " ";
    std::cout << "\n===================================================\n";

    //TODO : small number insert in Jacobsthal sequence.
    //Ford-Johnson insertion called!
    std::vector<int> res = sortedBigNumbers;
    if (!storedSmallNumPairs.empty())
    {
        res.insert(res.begin(), storedSmallNumPairs[0]);
    }
    // Jacobsthal sequence.
    std::vector<size_t> jacob;
    jacob.push_back(1);
    jacob.push_back(3);
    // 1,3,5,7 small number index
    while (jacob.back() < storedSmallNumPairs.size()) 
    {
        size_t size = jacob.size();
        jacob.push_back(NEXT_JACOB(jacob[size - 1], jacob[size - 2]));
    }

    //calculate order by index.
    //reverse : 2, 1, 3!
    std::vector<size_t> insertOrder;
    size_t lastPos = 1;
    for (size_t i = 1; i < jacob.size(); i++)
    {
        size_t stopPos = std::min((size_t)storedSmallNumPairs.size(), jacob[i]);
        for (size_t idx = stopPos; idx > lastPos; idx--)
        {
            insertOrder.push_back(idx - 1);
            std::cout << "insertOrder : " << idx - 1 << std::endl;
        }
        lastPos = jacob[i];
        if (lastPos >= storedSmallNumPairs.size())
            break;
    }

    //insert.
    for (size_t i : insertOrder)
    {
        int valToInsert = storedSmallNumPairs[i]; //
        // valToInsert의 짝인 sortedBigNumbers[i]의 위치까지만 탐색 범위로 한정 (Ford-Johnson 최적화)
        auto maxRange = std::find(res.begin(), res.end(), sortedBigNumbers[i]);
        auto pos = std::lower_bound(res.begin(), maxRange, valToInsert);
        res.insert(pos, valToInsert);
    }
 
    if (sadSingle.has_value())
    {
        auto pos = std::lower_bound(res.begin(), res.end(), sadSingle.value());
        res.insert(pos, sadSingle.value());
    }

    return res;
}

// std::vector<int> Sort::fordJohnsonInsertion(const std::vector<int> &bigs, const std::vector<int> &smalls)
// {
    
// }


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
