#include "pch.h"

#include "Aeyth8/Global.hpp"
#include "Aeyth8/Tools/Pointers.h"
#include "Aeyth8/Logic/Logic.h"

// My entire codebase has been designed to use namespaces like this.
using namespace A8CL; using namespace Global; using namespace Pointers;

static void PreInit()
{
	// Retrieves the Global Base Address (GBA) by getting the module handle casted as a uintptr_t
	GBA = (uintptr_t)GetModuleHandleA("GYMGAME-Win64-Shipping.exe");

	//LogWin();
	LogA("GetCommandLineA", GetCommandLineA());
	LogA("INITIALIZED", "The Global Base Address [GBA] is " + HexToString(GBA));

	Logic::Init_Hooks();
}

static void Init() {

	while (UWorld() == nullptr)
	{
		Sleep(2000);
	}
	
	Logic::Init_Vars(UWorld());

	if (!bConstructedUConsole) bConstructedUConsole = ConstructUConsole();

	// Changes the NetDriver to allow for LAN hosting. (I haven't worked on multiplayer yet, but I've tested it with the maploader and it allows me to join but nothing replicates.
	const static SDK::FName UIpNetDriver = FString2FName(L"OnlineSubsystemUtils.IpNetDriver");
	for (SDK::FNetDriverDefinition& Def : UEngine()->NetDriverDefinitions)
	{
		Def.DriverClassName = UIpNetDriver;
	}

	//Logic::InitGUI();

}

int __stdcall DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);

	if (ulReasonForCall != DLL_PROCESS_ATTACH) return 1;

	Global::InitLog();

	PreInit();

	if (Proxy::Attach(hModule))
		ConstructThread(Init);

	return 1;
}