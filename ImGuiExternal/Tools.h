#pragma once
#include<Windows.h>
#include<string>
#include<TlHelp32.h>

HANDLE sexroblox;
void setupchitchit(HANDLE concac)
{
	sexroblox = concac;
}
template <typename T>
T ReadMemoryEx(DWORD BaseAddress)
{

	T Buffer;
	ReadProcessMemory(sexroblox, (LPCVOID)BaseAddress, &Buffer, sizeof(Buffer), nullptr);

	return Buffer;
}

//std::wstring read_unicode(const std::uintptr_t address, std::size_t size)
//{
//	const auto buffer = std::make_unique<wchar_t[]>(size);
//
//	ReadProcessMemory(sexroblox, (LPVOID)address, buffer.get(), size * 2, 0);
//
//	return std::wstring(buffer.get());
//}
INT GetTrueProcessId(const char* ProcessName);
