#include "UnrealTypes.hpp"
#include "../Hooks/Hooks.hpp"
#include "../Offsets.h"
#include "../Global.hpp"
#include "../Tools/Pointers.h"

/*

Written by Aeyth8

https://github.com/Aeyth8

*/

using namespace A8CL; using namespace Global; using namespace Pointers;


void* FMemory::Malloc(size Count, uint32 Alignment)
{
	return OFF::FMalloc.VerifyFC<Decl::Malloc>()(Count, Alignment);
}

void* FMemory::Realloc(void* Original, size Count, uint32 Alignment)
{
	return OFF::FRealloc.VerifyFC<Decl::Realloc>()(Original, Count, Alignment);
}

void FMemory::Free(void* Original)
{
	OFF::FFree.VerifyFC<Decl::Free>()(Original);
}

size FMemory::GetAllocSize(void* Original)
{

}

void FMemory::Trim(bool bTrimThreadCaches)
{
	OFF::FTrim.VerifyFC<Decl::Trim>()(bTrimThreadCaches);
}

// Seems like it doesn't even get used
void* FMemory::GCreateMalloc()
{

}

// These are called either at startup or during a crash.
void* FMemory::MallocExternal(size Count, uint32 Alignment)
{
	//return OFF::FMallocExternal.VerifyFC<Decl::Malloc>()(Count, Alignment);
}

void* FMemory::ReallocExternal(void* Original, size Count, uint32 Alignment)
{
	return OFF::FRealloc.VerifyFC<Decl::Realloc>()(Original, Count, Alignment);
}

void FMemory::FreeExternal(void* Original)
{
	//OFF::FFreeExternal.VerifyFC<Decl::Free>()(Original);
}

size FMemory::GetAllocSizeExternal(void* Original)
{

}

size FMemory::QuantizeSizeExternal(size Count, uint32 Alignment)
{

}

/*
		FString
*/

/*std::string FString::ToString()
{

}*/