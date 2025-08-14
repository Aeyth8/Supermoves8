#include "UFunctions.hpp"
#include "../Hooks/Hooks.hpp"
#include "../Offsets.h"
#include "../Global.hpp"
#include "../Tools/Pointers.h"
#include "../../Dumper-7/SDK/GYMGAME_classes.hpp"
#include "UnrealTypes.hpp"

/*

Written by Aeyth8

https://github.com/Aeyth8

*/

using namespace A8CL;

/*
		Helpers
*/



const std::string& UFunctions::Helpers::FURLParser(SDK::FURL& URL)
{
	FURLParseCache.clear();

	// Parses FStrings
	for (int i{0}; i < 5; ++i)
	{
		if (&URL->*FURLPointers[i])
		{
			FURLParseCache += ("[" + FURLPointerNames[i] + "]: " + (URL.*FURLPointers[i]).ToString());

			// Combines the host and port
			if (i == 1 && URL.Port) FURLParseCache += ":" + std::to_string(URL.Port);
			FURLParseCache += " | ";
		}
	}
	if (&URL.Valid) FURLParseCache += ("[Valid]:" + std::to_string(URL.Valid) + " | ");

	if (URL.Op.Num() > 0) {
		FURLParseCache += "[Options]: ";
		for (int i{0}; i < URL.Op.Num(); ++i) FURLParseCache += ("?" + URL.Op[i].ToString());
		FURLParseCache += " | ";
	}
	
	return FURLParseCache;
}

const std::string& UFunctions::Helpers::FLPIParser(SDK::FFullyLoadedPackagesInfo& Info)
{
	FLPIParseCache.clear();

	FLPIParseCache += "\n[FullyLoadType]: " + FullyLoadPackageType[(int)Info.FullyLoadType];
	FLPIParseCache += " | [Tag]: " + Info.Tag.ToString() + " | [PackagesToLoad]: { ";

	for (int i{0}; i < Info.PackagesToLoad.Num(); ++i) FLPIParseCache += Info.PackagesToLoad[i].ToString() + " | ";
	FLPIParseCache += " } | [LoadedObjects]: { ";

	for (int i{0}; i < Info.LoadedObjects.Num(); ++i) FLPIParseCache +=  Info.LoadedObjects[i]->GetFullName() + " | ";
	FLPIParseCache += " }\n";

	return FLPIParseCache;
}

const std::string& UFunctions::Helpers::FLPIParser_T(SDK::TArray<SDK::FFullyLoadedPackagesInfo>& Info)
{
	std::string Return;

	for (int i{0}; i < Info.Num(); ++i) Return += UFunctions::Helpers::FLPIParser(Info[i]) + "\n";

	return Return;
}

const std::string& UFunctions::Helpers::FWorldContextParser(SDK::FWorldContext& Context)
{
	FWorldContextParseCache.clear();

	FWorldContextParseCache += ("\n[LastURL]: " + FURLParser(Context.LastURL));
	FWorldContextParseCache += ("\n[LastRemoteURL]: " + FURLParser(Context.LastRemoteURL));
	FWorldContextParseCache += ("\n[PackagesToFullyLoad]: " + FLPIParser_T(Context.PackagesToFullyLoad) + "\n[LoadedLevelsForPendingMapChange]: { ");

	for (int i{0}; i < Context.LoadedLevelsForPendingMapChange.Num(); ++i) FWorldContextParseCache += (Context.LoadedLevelsForPendingMapChange[i]->GetFullName() + " | ");
	FWorldContextParseCache += " }\n";

	// I don't think the rest is very important..

	return FWorldContextParseCache;
}

void UFunctions::Helpers::ProcessEnd()
{
	Hooks::DisableAllHooks();
	Hooks::Uninit(); 
	Global::CloseLog();
}


using namespace Global;


#include "../Logic/Logic.h"

typedef void(__fastcall* Import_T)(SDK::UGymMapImportControl* This, void* FJsonObject_MapData);
typedef void(__fastcall* ImportDBG_T)(SDK::UGymEditorControl* This, SDK::FString* File);

/*
		UFunctions
*/

void UFunctions::UConsole(SDK::UConsole* This, SDK::FString& Command)
{
	std::string StrCommand = Command.ToString();

	LogA("UConsole", StrCommand);

	if (StrCommand == "Dev")
	{
		static_cast<SDK::AGymPlayerController*>(Pointers::Player())->SetDevMode(true);
	}
	else if (StrCommand == "Unlock")
	{
		static_cast<SDK::AGymPlayerController*>(Pointers::Player())->DebugUnlockAllCareer(true);
	}
	else if (StrCommand == "Skin")
	{
		for (auto& P : Pointers::FindObjects<SDK::UGymCustomizationControl>())
		{
			P->DEBUGUnlockAll();
		}
	}
	else if (StrCommand == "Admin")
	{
		reinterpret_cast<SDK::UGymCloudApi*>(Pointers::UWorld()->OwningGameInstance)->AdminSession();
	}
	
		// ../Binaries/Win64/GYM_Maps/ 
		// Make a local path for loading any maps
		/*SDK::UGymEditorControl* Editor = Logic::EditorControl();
		if (!IsNull(Editor) && !(IsNull(Editor->Map)))
		{
			SDK::FString Name = L"Balls";
			OFF::DebugImport.VerifyFC<ImportDBG_T>()(Editor, &Name);

		}*/
	
	OFF::UConsole.VerifyFC<Decl::UConsole>()(This, Command);
}

SDK::FString* UFunctions::ConsoleCommand(SDK::APlayerController* This, SDK::FString* Result, SDK::FString* Command, bool bWriteToLog)
{
	std::string StrCommand = Command->ToString();

	if (StrCommand.find("loadmap") != std::string::npos)
	{
		Logic::HandleCustomMap(Command->CStr() + 8);
		/*std::thread([CustomMap]() {
			Logic::HandleCustomMap(CustomMap);
			}).detach();*/
	}

	return OFF::ConsoleCommand.VerifyFC<Decl::ConsoleCommand>()(This, Result, Command, bWriteToLog);
}

UFunctions::BrowseReturnVal UFunctions::Browse(SDK::UEngine* This, SDK::FWorldContext& WorldContext, SDK::FURL URL, SDK::FString& Error)
{
	if (!Global::bConstructedUConsole) { Global::bConstructedUConsole = Pointers::ConstructUConsole();
		LogA("Browse", "Constructed UConsole early.");
	}

	LogA("Browse", Helpers::FURLParser(URL));
	//LogA("Browse", Helpers::FWorldContextParser(WorldContext));

	return OFF::Browse.VerifyFC<Decl::Browse>()(This, WorldContext, URL, Error);
}

bool UFunctions::InitListen(SDK::UIpNetDriver* This, SDK::UObject* InNotify, SDK::FURL& LocalURL, bool bReuseAddressAndPort, SDK::FString& Error)
{
	//return VerifyFC<Decl::InitListen>(OFF::InitListen)(This, InNotify, LocalURL, bReuseAddressAndPort, Error);
}

void UFunctions::PreLogin(SDK::AGameModeBase* This, SDK::FString* Options, SDK::FString* Address, SDK::FUniqueNetIdRepl* UniqueId, SDK::FString* ErrorMessage)
{
	LogA("PreLogin", "Options: " + Options->ToString() + " Address: " + Address->ToString());
}

void UFunctions::AppPreExit()
{
	Global::ConstructThread(Helpers::ProcessEnd);
	OFF::AppPreExit.VerifyFC<Decl::AppPreExit>()();
}

void UFunctions::SpawnActor(SDK::UWorld* This, SDK::UClass* Class, const SDK::FVector* Location, const SDK::FRotator* Rotation, FActorSpawnParameters* SpawnParameters)
{



}

void UFunctions::ProcessEvent(SDK::UObject* This, SDK::UFunction* Function, LPVOID Parms)
{



}