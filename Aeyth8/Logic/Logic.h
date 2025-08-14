#pragma once


/*

Written by Aeyth8

https://github.com/Aeyth8

*/


namespace SDK
{
	class UWorld;
	
	class UGymGameInstance;
	class UGymEditorControl;

	class AGymMap;
	class AGymPlayerController;
	class AGymCharacter;
	
	
	
}

namespace A8CL
{
namespace Logic
{
	// -- Vars

	extern SDK::UGymGameInstance* GameInstance;

	// -- Initialization

	void Init_Hooks();
	void Init_Vars(SDK::UWorld* GWorld);
	//void InitGUI();

	// -- Pointers

	SDK::AGymPlayerController* Player(const int& Index = 0);
	SDK::UGymEditorControl* EditorControl(const int& Index = 0);
	SDK::AGymMap* const& GetCurrentMap();

	void HandleCustomMap(const wchar_t* Command);
}
}