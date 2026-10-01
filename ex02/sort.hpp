#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <deque>
#include <stdexcept>
#include <optional>

/*
    n개의 원소를 2개씩 짝 짓는다...
    홀수개일 경우 마지막 1개는 따로 보관한다..
    각쌍마다 두 수를 비교해서 큰값과 작은값으로 나눈다...
    (small, big), (small, big), (small, big)...

    recursive Sort 
    big 만 모아서 다시 merge-insertion sort 재귀호출


*/
class Sort
{
public:
    Sort(int argc, char *argv[]);
    Sort(const Sort &other) = default;
    Sort &operator=(const Sort &other) = default;
    Sort() = default;
    ~Sort() = default;

    void printTest();
private:
    bool _isSwapped = false;
    std::vector<int> _vec;
    std::deque<int> _deq;
    std::vector<std::pair<int, int>> makePairs(const std::vector<int> &vpArr, std::optional<int> &sadSingle);
    int fromChartoInt(const std::string &str);
};