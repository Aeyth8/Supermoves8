#pragma once
#include "../Global.hpp"
#include "../../Dumper-7/SDK/Engine_classes.hpp"
#include <vector>

/*

Written by Aeyth8

https://github.com/Aeyth8

*/


namespace A8CL
{





class FMath
{
public:
	template <class T>
	inline constexpr T Max(const T A, const T B) { return (A >= B) ? A : B; }

	template <class T>
	inline constexpr T Clamp(const T X, const T Min, const T Max) { return X < Min ? Min : X < Max ? X : Max; }

	template <class T, class U>
	inline constexpr T Lerp(const T& A, const T& B, const U& Alpha) { return (T)(A + Alpha * (B - A)); }
};

struct FMemory
{
	class Decl
	{
	public:
		typedef void*(__fastcall* Malloc)(size Count, uint32 Alignment);
		typedef void*(__fastcall* Realloc)(void* Original, size Count, uint32 Alignment);
		typedef void(__fastcall* Free)(void* Original);
		typedef void(__fastcall* Trim)(bool bTrimThreadCaches);


	};

	enum AllocationHints
	{
		None = -1,
		Default,
		Temporary,
		SmallPool,
		Max
	};

	enum
	{
		// Default allocator alignment. If the default is specified, the allocator applies to engine rules.
		// Blocks >= 16 bytes will be 16-byte-aligned, Blocks < 16 will be 8-byte aligned. 
		// If the allocator does not support allocation alignment, the alignment will be ignored.
		DEFAULT_ALIGNMENT = 0,

		// Minimum allocator alignment
		MIN_ALIGNMENT = 8,
	};



	// C-Style Memory Allocation (The most important part)

	static void* Malloc(size Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void* Realloc(void* Original, size Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void Free(void* Original);
	static size GetAllocSize(void* Original);
	static void Trim(bool bTrimThreadCaches = true);

	/*
		I won't implement the more useless crappy ones unless I actually need them (I will need them)
	*/

	static void* GCreateMalloc(); // Seems like it doesn't even get used

	// These are called either at startup or during a crash.
	static void* MallocExternal(size Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void* ReallocExternal(void* Original, size Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void FreeExternal(void* Original);
	static size GetAllocSizeExternal(void* Original);
	static size QuantizeSizeExternal(size Count, uint32 Alignment = DEFAULT_ALIGNMENT);

};

class FNetworkNotify
{
	enum EAcceptConnection
	{
		Reject = 0,
		Accept = 1,
		Ignore = 2
	};

	static EAcceptConnection NotifyAcceptingConnection();

	static void NotifyAcceptedConnection();

	static void NotifyAcceptingChannel();

	static void NotifyControlMessage();



};

enum EConnectionState
{
	USOCK_Invalid = 0,
	USOCK_Closed  = 1,
	USOCK_Pending = 2,
	USOCK_Open    = 3, 
};

struct FNetworkObjectInfo
{
	SDK::AActor* Actor;

	// Stupid weakpointer bull I don't UNDERSTAND WHAT IS A WEAK POINTER, BE SELF EXPLANATORY

	double NextUpdateTime;

	double LastNetReplicateTime;

	float OptimalNetUpdateDelta;

	float LastNetUpdateTime;

	uint32 bPendingNetUpdate : 1;

	uint32 bForceRelevantUpdate : 1;

	// Not doing these stupid TSet hunk of mongrel garbage until I fully understand it  

	// Default constructor
	FNetworkObjectInfo() 
	: Actor(nullptr)
	, NextUpdateTime(0.0)
	, LastNetReplicateTime(0.0)
	, OptimalNetUpdateDelta(0.0f)
	, LastNetUpdateTime(0.0f)
	, bPendingNetUpdate(false)
	, bForceRelevantUpdate(false) {}
	

	FNetworkObjectInfo(SDK::AActor* InActor) 
	: Actor(InActor)
	, NextUpdateTime(0.0)
	, LastNetReplicateTime(0.0)
	, OptimalNetUpdateDelta(0.0f)
	, LastNetUpdateTime(0.0f)
	, bPendingNetUpdate(false)
	, bForceRelevantUpdate(false) {}
};

enum EWorldType
{
	/** An untyped world, in most cases this will be the vestigial worlds of streamed in sub-levels */
	None,

	/** The game world */
	Game,

	/** A world being edited in the editor */
	Editor,

	/** A Play In Editor world */
	PIE,

	/** A preview world for an editor tool */
	EditorPreview,

	/** A preview world for a game */
	GamePreview,

	/** An editor world that was loaded but not currently being edited in the level editor */
	Inactive
};

enum ENetMode 
{
	NM_Standalone,
	NM_DedicatedServer,
	NM_ListenServer,
	NM_Client,
	NM_MAX,
	Passthrough // Custom mode
};

enum ENetRole
{
	ROLE_None,
	ROLE_SimulatedProxy, // Default client role
	ROLE_AutonomousProxy,
	ROLE_Authority,
	ROLE_MAX
};

/*template <class ArrayElementType>
class TArray
{
public:

protected:

	ArrayElementType* Data;
	int32 NumElements;
	int32 MaxElements;

public:

	TArray()

};


class FString : public std::vector<wchar_t>
{
public:


	FString(const std::string& String)
	{
		(FString*)FMemory::Malloc(sizeof(String), 0);
	}

	std::string ToString();
	std::wstring ToWString();
	wchar_t* CStr();

};*/



}