#pragma once
#include <vector>
#include <Windows.h>
#include "ProcessManager.h"

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}

template<typename T> T RPM(SIZE_T address) {
	//The buffer for data that is going to be read from memory
	T buffer;
	//The actual RPM
	ReadProcessMemory(process::hProc, (LPCVOID)address, &buffer, sizeof(T), NULL);

	//Return our buffer
	return buffer;
}