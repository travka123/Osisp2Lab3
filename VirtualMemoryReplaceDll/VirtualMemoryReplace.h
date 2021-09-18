#pragma once

#include <Windows.h>

#ifdef VirtualMemoryReplace_EXPORTS
#define VirtualMemoryReplace_API __declspec(dllexport)
#else
#define VirtualMemoryReplace_API __declspec(dllimport)
#endif

struct Params {
	DWORD pid;
	char oldstr[256]; 
	char newstr[256]; 
	int oldstrSize; 
	int newstrSize;
};

extern "C" VirtualMemoryReplace_API void VirtualMemoryReplace(Params *params);