#include "pch.h" 

#include <Windows.h>
#include <string>

#include "VirtualMemoryReplace.h";

char* SearchString(char* buffer, int bufferSize, const char* str, int strlen) {
	for (int i = 0; bufferSize - i >= strlen; i++) {
		if (!strncmp(buffer + i, str, strlen)) {
			return buffer + i;
		}
	}
	return nullptr;
}

void RegionReplace(HANDLE hProcess, MEMORY_BASIC_INFORMATION* memInfo,
	char* oldstr, char* newstr, int oldstrSize, int newstrSize) {

	SIZE_T bytes;
	char* buffer = new char[memInfo->RegionSize];

	oldstr[0]++; //Save str from replace
	ReadProcessMemory(hProcess, memInfo->BaseAddress, buffer, memInfo->RegionSize, &bytes);
	oldstr[0]--; //Remove saving

	char* curBuffer = buffer;
	char* strpos = SearchString(curBuffer, memInfo->RegionSize, oldstr, oldstrSize);
	while (strpos) {
		WriteProcessMemory(hProcess, (char*)memInfo->BaseAddress + (strpos - buffer), newstr, newstrSize, nullptr);
		curBuffer = strpos + oldstrSize;
		strpos = SearchString(curBuffer, memInfo->RegionSize - (curBuffer - buffer), oldstr, oldstrSize);
	}
	delete[] buffer;
}

void VirtualMemoryReplace(Params *params) {
	if (!(params->newstrSize || params->oldstrSize)) return;

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, false, params->pid);
	char* curAddress = (char*)sysInfo.lpMinimumApplicationAddress;

	while (curAddress < sysInfo.lpMaximumApplicationAddress) {
		MEMORY_BASIC_INFORMATION memInfo;
		VirtualQueryEx(hProcess, curAddress, &memInfo, sizeof(memInfo));

		if (memInfo.State == MEM_COMMIT && memInfo.AllocationProtect == PAGE_READWRITE) {
			RegionReplace(hProcess, &memInfo, params->oldstr, params->newstr, params->oldstrSize, params->newstrSize);
		}
		curAddress += memInfo.RegionSize;
	}
}