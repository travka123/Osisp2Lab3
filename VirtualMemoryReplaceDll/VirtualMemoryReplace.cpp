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
	ReadProcessMemory(hProcess, memInfo->BaseAddress, buffer, memInfo->RegionSize, &bytes);

	oldstr[0]--; //Remove saving

	int left = memInfo->RegionSize;
	char* curBuffer = buffer;
	char* strpos = SearchString(curBuffer, left, oldstr, oldstrSize);
	while (strpos) {
		WriteProcessMemory(hProcess, (char*)memInfo->BaseAddress + (strpos - buffer), newstr, newstrSize + 1, nullptr);
		curBuffer = strpos + oldstrSize;
		strpos = SearchString(curBuffer, memInfo->RegionSize - (curBuffer - buffer), oldstr, oldstrSize);
	}

	oldstr[0]++; //Save str from replace

	delete[] buffer;
}

void VirtualMemoryReplace(DWORD pid, void* minimumApplicationAddress, void* maximumApplicationAddress,
	char* oldstr, char* newstr, int oldstrSize, int newstrSize) {

	if (!(oldstrSize || newstrSize)) return;

	HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, false, pid);
	char* curAddress = (char*)minimumApplicationAddress;

	oldstr[0]++; //Save str from replace
	while (curAddress < maximumApplicationAddress) {
		MEMORY_BASIC_INFORMATION memInfo;
		VirtualQueryEx(hProcess, curAddress, &memInfo, sizeof(memInfo));

		if (memInfo.State == MEM_COMMIT && memInfo.AllocationProtect == PAGE_READWRITE) {
			RegionReplace(hProcess, &memInfo, oldstr, newstr, oldstrSize, newstrSize);
		}

		curAddress += memInfo.RegionSize;
	}
	oldstr[0]--; //Remove saving
}