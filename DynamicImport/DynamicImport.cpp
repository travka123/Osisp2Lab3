#include <iostream>
#include <Windows.h>

int main()
{
    HINSTANCE hInstance = LoadLibrary(L"VirtualMemoryReplaceDll.dll");

    if (!hInstance) {
        std::cout << "VirtualMemoryReplaceDll.dll not found";
        return 1;
    }

    void (*VirtualMemoryReplace)(DWORD pid, void* minimumApplicationAddress, void* maximumApplicationAddress,
        char* oldstr, char* newstr, int oldstrSize, int newstrSize);

    VirtualMemoryReplace = (void (*)(DWORD pid, void* minimumApplicationAddress, void* maximumApplicationAddress,
            char* oldstr, char* newstr, int oldstrSize, int newstrSize))GetProcAddress(hInstance, "VirtualMemoryReplace");

    if (!VirtualMemoryReplace) {
        std::cout << "VirtualMemoryReplace func not found";
        return 1;
    }

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

    return 0;
}


