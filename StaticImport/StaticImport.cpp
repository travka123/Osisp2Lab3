#include <iostream>

#include "VirtualMemoryReplace.h"

int main()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);


    char forFind[] = "Hello world";
    char forReplace[] = "Hello user!";

    char test1[] = "Hello world";
    std::string test2 = "Hello world";
    std::string test3 = " ((( Hello world )))";

    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;

    VirtualMemoryReplace(GetCurrentProcessId(), sysInfo.lpMinimumApplicationAddress, sysInfo.lpMaximumApplicationAddress, 
        forFind, forReplace, strlen(forFind), strlen(forReplace));

    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;

    std::cin.get();
}


