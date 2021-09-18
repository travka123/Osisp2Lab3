#include <iostream>

#include "VirtualMemoryReplace.h"

int main()
{
    Params params;
    params.pid = GetCurrentProcessId();
    strcpy_s(params.oldstr, "Hello world");
    strcpy_s(params.newstr, "Hello user!");
    params.oldstrSize = strlen("Hello world");
    params.newstrSize = strlen("Hello user!");

    char test1[] = "Hello world";
    std::string test2 = "Hello world";
    std::string test3 = " ((( Hello world )))";

    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;

    VirtualMemoryReplace(&params);

    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;

    std::cin.get();
}


