#include "sort.hpp"

int main(int argc, char *argv[])
{
    const char *testArr[] = {"./sort", "1", "2", "6", "7", "3", "9", "8"};
    (void)argv;
    argc = sizeof(testArr) / sizeof(testArr[0]);
    try
    {
        Sort sort(argc, const_cast<char**>(testArr));
        sort.printTest();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
