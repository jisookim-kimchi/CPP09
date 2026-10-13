#include "pMergeMe.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Error" << std::endl;
        return 1;
    }
    try
    {
        pMergeMe pMerged_sort(argc, argv);
        pMerged_sort.printResult();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
