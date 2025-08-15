#include "Hooks/Hooks.hpp"

class OFFSET;

namespace A8CL
{
namespace OFF
{
	// Basic UE Functions

	OFFSET FMalloc("FMemory::Malloc", 0xBA0940);
	OFFSET FRealloc("FMemory::Realloc", 0xBA31F0);
	OFFSET FFree("FMemory::Free", 0xB93C10);
	OFFSET FTrim("FMemory::Trim", 0xBACA30);

	OFFSET AppPreExit("FEngineLoop::AppPreExit", 0x368B370);

	OFFSET Browse("UEngine::Browse", 0x35694A0);
	OFFSET UConsole("UConsole::ConsoleCommand", 0x35A4F90);
	OFFSET ConsoleCommand("APlayerController::ConsoleCommand", 0x330BB90);
	OFFSET PreLogin("AGameModeBase::PreLogin", 0x2F543E0);
	OFFSET Login("AGameModeBase::Login", 0x2F4F130);

	OFFSET CopyString("FString::FString", 0xB45490);
	
	// Native Game Functions
	
	OFFSET Import("UGymMapImportControl::Import", 0x43D8130);
	OFFSET DebugImport("UGymEditorControl::DebugImport_Implementation", 0x42E2F70);
	OFFSET EditorConstructor("UGymEditorControl::UGymEditorControl", 0x42D9F00);
	OFFSET SetParentMap("AGymStaticLevelAsset::SetParentMap", 0x4337930);
	OFFSET MapBP("AGymMap::BeginPlay", 0x43B9B30);
	OFFSET FEmptyDelegateWrapper("FEmptyOnlineDelegate_DelegateWrapper", 0x28C93F0);
}
}