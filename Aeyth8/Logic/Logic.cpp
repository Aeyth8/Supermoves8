#include "Logic.h"
#include "../Global.hpp"
#include "../Hooks/Hooks.hpp"
#include "../Offsets.h"

#include "../Tools/Pointers.h"
#include "../Tools/UFunctions.hpp"
#include "../Tools/BytePatcher.h"

#include "../../Dumper-7/SDK/GYMGAME_classes.hpp"

#include "../NT/NTFunctions.h"
#include "../Proxy8/DirectXHook.h"
#include <winternl.h>
#include "../Tools/UnrealTypes.hpp"

/*

Written by Aeyth8

https://github.com/Aeyth8

*/


using namespace A8CL; using namespace Global;


// -- Vars

SDK::UGymGameInstance* Logic::GameInstance{nullptr};

// -- Constants

constexpr const BYTE NOP{0x90};
constexpr const BYTE JMP{0xEB};

// -- Hooks



typedef NTSTATUS(__stdcall* LdrLoadDll_T)(_In_opt_ PCWSTR DllPath, _In_opt_ PULONG DllCharacteristics, _In_ PCUNICODE_STRING DllName, _Out_ PVOID* DllHandle);
LdrLoadDll_T FC_LdrLoadDll{0};

static NTSTATUS __stdcall LdrLoadDll(_In_opt_ PCWSTR DllPath, _In_opt_ PULONG DllCharacteristics, _In_ PCUNICODE_STRING DllName, _Out_ PVOID* DllHandle)
{
	char Name[260]{0};
	wcstombs_s(0,Name, DllName->Buffer, lstrlenW(DllName->Buffer));

	std::string StrName(Name);
	//LogA("LdrLoadDll", StrName);

	// If you prevent the steamclient64.dll from loading it will fail to load the cloudsaves which includes your character and progress, meaning parts of the progress/account are still tied online 
	if (StrName.rfind("gameoverlayrenderer64") != std::string::npos /*|| StrName.rfind("steamclient64.dll") != std::string::npos*/) return 0xC0000005;
	
	return FC_LdrLoadDll(DllPath, DllCharacteristics, DllName, DllHandle);
}

typedef void(__fastcall* ImportDBG_T)(SDK::UGymEditorControl* This, SDK::FString* File);

struct MapNames
{
	const wchar_t* PackageName;
	const wchar_t* ShortenedName;
};

constexpr const MapNames MapNameIndex[] =
{
	{L"Courtyard_Empty", L"Courtyard"},
	{L"Flatlands_Empty", L"Flatlands"},
	{L"Ocean_Empty", L"Ocean"},
	{L"Plywood_Empty", L"Warehouse"},
	{L"Rooftops_Emilia", L"Rooftops"},
	{L"Schoolyard_Empty", L"Schoolyard"},
	{L"Stadium_Large_Empty", L"StadiumL"},
	{L"Stadium_Medium_Empty", L"StadiumM"},
	{L"Stadium_Small_Empty", L"Stadium"},
	{L"Streets_Empty", L"Streets"},
	{L"Tutorial_Empty", L"Gym"},
	{L"TournamentLobby", L"Tournament"},
};

static SDK::FString GMapDirectory{};
static SDK::UGymEditorControl* GEditorControl{nullptr};
static bool bLoadCustomMap{false};


typedef void(__fastcall* EditorConstructor_T)(SDK::UGymEditorControl* This);
void __fastcall EditorConstruct(SDK::UGymEditorControl* This)
{
	OFF::EditorConstructor.VerifyFC<EditorConstructor_T>()(This);

	GEditorControl = This;
}

typedef void(__fastcall* MapBP_T)(SDK::AGymMap* This);
void __fastcall MapBP(SDK::AGymMap* This)
{
	OFF::MapBP.VerifyFC<MapBP_T>()(This);

	LogA(OFF::MapBP.GetName(), This->GetFullName());

	if (bLoadCustomMap)
	{
		bLoadCustomMap = false;

		if (GEditorControl && This == GEditorControl->Map)
		{
			LogA("MapLoader", GMapDirectory.ToString());
			Call<ImportDBG_T>(OFF::DebugImport.PlusBase())(GEditorControl, &GMapDirectory);
		}

	}
}

// Normal formatting example is loadmap 1472?Ocean
void Logic::HandleCustomMap(const wchar_t* Command)
{
    wchar_t CurrentDirectory[260]{0};
    if (!GetModuleFileNameW(GetModuleHandleW(nullptr), CurrentDirectory, _countof(CurrentDirectory)))
    {
        LogA("HandleCustomMap", "Failed to load map because the program was unable to retrieve the local path of the directory");
        return;
    }

    wchar_t* LastBackslash = wcsrchr(CurrentDirectory, L'\\');
    if (LastBackslash)
		*(LastBackslash + 1) = L'\0';

    wchar_t MapId[20]{0};
    const wchar_t* CommandEnd = wcschr(Command, L'?');

    if (CommandEnd)
    {
        size_t Index = CommandEnd - Command;
        wcsncpy_s(MapId, _countof(MapId), Command, Index);
    }
    else
    {
        wcscpy_s(MapId, _countof(MapId), Command);
    }

    wchar_t MapOverride[50]{0};
    if (CommandEnd)
    {
        wcscpy_s(MapOverride, _countof(MapOverride), CommandEnd + 1);
    }

    wcscat_s(CurrentDirectory, _countof(CurrentDirectory), L"GYM_Maps\\");
    wcscat_s(CurrentDirectory, _countof(CurrentDirectory), MapId);

	//SDK::FString MapDirectory(CurrentDirectory);
	//LogA("MapDirectory", MapDirectory.ToString());

    if (GetFileAttributesW(CurrentDirectory) != INVALID_FILE_ATTRIBUTES)
    {
        

        if (MapOverride[0] != L'\0')
        {
            wchar_t OpenMapCommand[50]{0};
            wcscpy_s(OpenMapCommand, _countof(OpenMapCommand), L"open ");
            wcscat_s(OpenMapCommand, _countof(OpenMapCommand), MapOverride);
			SDK::FString TheMapCommand{OpenMapCommand};

			wchar_t MapLocal[260]{0};
			wcscpy_s(MapLocal, _countof(MapLocal), L"..\\Binaries\\Win64\\GYM_Maps\\");				
			wcscat_s(MapLocal, _countof(MapLocal), MapId);

			SDK::FString LocalDirectory{MapLocal};
			Call<UFunctions::Decl::CopyString>(OFF::CopyString.PlusBase())(&GMapDirectory, &LocalDirectory);

			bLoadCustomMap = true;

            static SDK::UKismetSystemLibrary* Lib = SDK::UKismetSystemLibrary::GetDefaultObj();
			Lib->ExecuteConsoleCommand(Pointers::UWorld(), TheMapCommand, Pointers::Player());
        }
    }
}

static std::vector<Hooks::HookStructure> HookList =
{
	{OFF::UConsole, UFunctions::UConsole},
	{OFF::Browse, UFunctions::Browse},
	{OFF::AppPreExit, UFunctions::AppPreExit},
	{OFF::PreLogin, UFunctions::PreLogin},
	{OFF::ConsoleCommand, UFunctions::ConsoleCommand},
	{OFF::EditorConstructor, EditorConstruct},
	{OFF::MapBP, MapBP},
};

void Logic::Init_Hooks()
{
	if (Hooks::Init())
	{
		/*uintptr_t NT = (uintptr_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "LdrLoadDll");
		MH_CreateHook((LPVOID)NT, LdrLoadDll, (LPVOID*)&FC_LdrLoadDll);
		MH_EnableHook((LPVOID)NT);*/

		Hooks::CreateAndEnableHooks(HookList);		
	}
}

void Logic::Init_Vars(SDK::UWorld* GWorld)
{
	if (!IsNull(GWorld))
	{
		Logic::GameInstance = static_cast<SDK::UGymGameInstance*>(GWorld->OwningGameInstance);
	}
}


// -- Pointers

SDK::AGymPlayerController* Logic::Player(const int& Index)
{
	SDK::APlayerController* Player = Pointers::Player();

	if (!IsNull(Player))
	{
		if (Player->IsA(SDK::AGymPlayerController::StaticClass()))
		{
			return static_cast<SDK::AGymPlayerController*>(Pointers::Player(Index));
		}

		LogA("Pointers", "GymPlayerController is not of type! PlayerController is not nullptr.");
		return nullptr;
	}

	LogA("Pointers", "GymPlayerController is a nullptr!");
	return nullptr;
}

SDK::UGymEditorControl* Logic::EditorControl(const int& Index)
{
	SDK::AGymPlayerController* Controller = Logic::Player(Index);

	if (!IsNull(Controller) && !IsNull(Controller->GetGymCharacter()))
	{
		return Controller->GetGymCharacter()->GetEditorControl();
	}

	LogA("Pointers", "GymEditorControl is a nullptr!");
	return nullptr;
}

SDK::AGymMap* const& Logic::GetCurrentMap()
{
	if (Logic::GameInstance)
	{
		return Logic::GameInstance->CurrentGymMap;
	}

	LogA("Pointers", "Current AGymMap is a nullptr!");
	return nullptr;
}