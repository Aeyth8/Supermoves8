#include "NTFunctions.h"
#include "NT.h"
#include "../Global.hpp"

/*

Written by Aeyth8

https://github.com/Aeyth8

*/


NTSTATUS NTFunctions::LdrLoadDll(PCWSTR PathToFile, PULONG Flags, PCUNICODE_STRING ModuleFileName, PVOID* ModuleHandle)
{
	char cName[260]{0};
	wcstombs_s(0, cName, ModuleFileName->Buffer, lstrlenW(ModuleFileName->Buffer));
	A8CL::Global::LogA("LdrLoadDll", std::string(cName));

	return reinterpret_cast<Decl::LdrLoadDll>(Decl::FC_LdrLoadDll)(PathToFile, Flags, ModuleFileName, ModuleHandle);
}