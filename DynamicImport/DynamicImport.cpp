#include <iostream>
#include <Windows.h>

struct Params {
    DWORD pid;
    char oldstr[256];
    char newstr[256];
    int oldstrSize;
    int newstrSize;
};

int main()
{
    HINSTANCE hInstance = LoadLibrary(L"VirtualMemoryReplaceDll.dll");

    if (!hInstance) {
        std::cout << "VirtualMemoryReplaceDll.dll not found";
        return 1;
    }

    void (*VirtualMemoryReplace)(Params *params);

    VirtualMemoryReplace = (void (*)(Params*))GetProcAddress(hInstance, "VirtualMemoryReplace");

    if (!VirtualMemoryReplace) {
        std::cout << "VirtualMemoryReplace func not found";
        return 1;
    }

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

    return 0;
}


