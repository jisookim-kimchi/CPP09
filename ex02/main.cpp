#include "sort.hpp"

int main(int argc, char *argv[])
{
    try
    {
        Sort sort(argc, argv);
        sort.printTest();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
