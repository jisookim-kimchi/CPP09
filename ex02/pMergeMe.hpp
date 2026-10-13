#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <deque>
#include <stdexcept>
#include <optional>
#include <algorithm>
#include <chrono>
#include <iomanip>
/*
    n개의 원소를 2개씩 짝 짓는다...
    홀수개일 경우 마지막 1개는 따로 보관한다..
    각쌍마다 두 수를 비교해서 큰값과 작은값으로 나눈다...
    (small, big), (small, big), (small, big)...

    recursive Sort 
    big 만 모아서 다시 merge-insertion sort 재귀호출


*/

/**
    @a : previous jacob number
    @b : before previous jacob number
*/
#define NEXT_JACOB(a, b) ((a) + 2 * (b))

class pMergeMe
{
public:
    pMergeMe(int argc, char *argv[]);
    pMergeMe(const pMergeMe &other) = default;
    pMergeMe &operator=(const pMergeMe &other) = default;
    pMergeMe() = default;
    ~pMergeMe() = default;

    void printResult();

    template <typename Container>
    Container mergeInsertionSort(const Container &arr);

    template <typename Container>
    std::vector<std::pair<int, int>> makePairs(const Container &vpArr, std::optional<int> &sadSingle);

    template <typename Container>
    std::pair<Container, double> measureSort(const Container &container)
    {
        auto start = std::chrono::steady_clock::now();
        Container sorted = mergeInsertionSort(container);
        auto end = std::chrono::steady_clock::now();
        double elapsedMicroseconds = std::chrono::duration<double, std::micro>(end - start).count();
        return {sorted, elapsedMicroseconds};
    }

private:
    bool _isSwapped = false;
    std::vector<int> _vec;
    std::deque<int> _deq;
    int fromChartoInt(const std::string &str);
};

template <typename Container>
std::vector<std::pair<int, int>> pMergeMe::makePairs(const Container &vpArr, std::optional<int> &sadSingle)
{
    std::vector<std::pair<int, int>> pairs;

    for (auto it = vpArr.begin(); it != vpArr.end() && (it + 1 != vpArr.end()); it += 2)
    {
        int first = *it;
        int second = *(it + 1);
        if (first > second)
            pairs.push_back({second, first});
        else
            pairs.push_back({first, second});
    }
    if (vpArr.size() % 2 != 0)
        sadSingle = vpArr.back();
    return pairs;
}

template <typename Container>
Container pMergeMe::mergeInsertionSort(const Container &container)
{
    if (container.size() <= 1)
        return container;

    std::optional<int> sadSingle;
    std::vector<std::pair<int, int>> pairs = makePairs(container, sadSingle);
    Container bigNums;
    for (const auto &p : pairs)
    {
        bigNums.push_back(p.second);
    }

    Container BigNumbers = mergeInsertionSort(bigNums);
    Container SmallNumbers;
    for (const auto &big : BigNumbers)
    {
        for (auto it = pairs.begin(); it != pairs.end(); ++it)
        {
            if (it->second == big)
            {
                SmallNumbers.push_back(it->first);
                pairs.erase(it);
                break;
            }
        }
    }

    Container res = BigNumbers;
    if (!SmallNumbers.empty())
    {
        res.insert(res.begin(), SmallNumbers[0]);
    }

    // Jacobsthal sequence
    std::vector<size_t> jacob;
    jacob.push_back(1);
    jacob.push_back(3);
    while (jacob.back() < SmallNumbers.size())
    {
        size_t size = jacob.size();
        jacob.push_back(NEXT_JACOB(jacob[size - 1], jacob[size - 2]));
    }

    std::vector<size_t> insertOrder;
    size_t lastPos = 1;
    for (size_t i = 1; i < jacob.size(); i++)
    {
        size_t stopPos = std::min((size_t)SmallNumbers.size(), jacob[i]);
        for (size_t idx = stopPos; idx > lastPos; idx--)
        {
            insertOrder.push_back(idx - 1);
        }
        lastPos = jacob[i];
        if (lastPos >= SmallNumbers.size())
            break;
    }

    for (size_t i : insertOrder)
    {
        int valToInsert = SmallNumbers[i];
        auto maxRange = std::find(res.begin(), res.end(), BigNumbers[i]);
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