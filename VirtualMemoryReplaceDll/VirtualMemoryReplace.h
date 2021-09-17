#pragma once

#include <Windows.h>

#ifdef VirtualMemoryReplace_EXPORTS
#define VirtualMemoryReplace_API __declspec(dllexport)
#else
#define VirtualMemoryReplace_API __declspec(dllimport)
#endif

extern "C" VirtualMemoryReplace_API void VirtualMemoryReplace(DWORD pid, void* minimumApplicationAddress, void* maximumApplicationAddress,
	char* oldstr, char* newstr, int oldstrSize, int newstrSize);