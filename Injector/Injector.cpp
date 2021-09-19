#include <Windows.h>
#include <tlhelp32.h>

#include <string>
#include <iostream>

struct Params {
    DWORD pid;
    char oldstr[256];
    char newstr[256];
    int oldstrSize;
    int newstrSize;
};

DWORD GetPid(wchar_t* exeName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    DWORD targetPid = 0;
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    while (Process32Next(hSnapshot, &pe)) {
        if (!wcscmp(exeName, pe.szExeFile)) {
            targetPid = pe.th32ProcessID;
            break;
        }
    }
    CloseHandle(hSnapshot);
    return targetPid;
}

bool LoadRemote(DWORD pid, const wchar_t* dllName, const char* funcName, const void* lpParameter, int parameterSize) {
    if (!(pid && dllName && funcName)) return false;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (!hProcess) return false;

    LPVOID func = (LPVOID)GetProcAddress(GetModuleHandle(dllName), funcName);
    if (!func) return false;

    LPVOID mem = (LPVOID)VirtualAllocEx(hProcess, 0, parameterSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!mem) return false;

    if (!WriteProcessMemory(hProcess, (LPVOID)mem, lpParameter, parameterSize, 0)) { return false; }

    HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)func, (LPVOID)mem, 0, 0);
    if (hThread == NULL) return false;

    CloseHandle(hProcess);

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return true;
}

int main()
{
    wchar_t pName[256];
    std::wcout << "exe file name: ";
    std::wcin.getline(pName, 256, '\n');

    DWORD targetPid = GetPid(pName);

    if (!targetPid) {
        std::cout << "Process not found" << std::endl;
        return 1;
    }

    char oldstr[256], newstr[256];
    std::cout << "old string:" << std::endl;
    std::cin.getline(oldstr, 256, '\n');
    std::cout << "new string:" << std::endl;
    std::cin.getline(newstr, 256, '\n');

    Params params;
    params.pid = targetPid;
    strcpy_s(params.oldstr, oldstr);
    strcpy_s(params.newstr, newstr);
    params.oldstrSize = strlen(oldstr);
    params.newstrSize = strlen(newstr);

    HINSTANCE hInstance = LoadLibraryA("VirtualMemoryReplaceDll.dll");

    if (!hInstance) {
        std::cout << "Dll not found" << std::endl;
        return 1;
    }

    void (*SetInvokeOnLoad)(bool invokeOnLoad, Params * params) = (void (*)(bool, Params*))GetProcAddress(hInstance, "SetInvokeOnLoad");

    SetInvokeOnLoad(true, &params);

    if (!LoadRemote(targetPid, L"kernel32.dll", "LoadLibraryA", "VirtualMemoryReplaceDll.dll", strlen("VirtualMemoryReplaceDll.dll") + 1)) {
        std::cout << "error" << std::endl;
        return 1;
    }

    SetInvokeOnLoad(false, nullptr);
    
    std::cout << "OK" << std::endl;
    std::cin.get();
}

