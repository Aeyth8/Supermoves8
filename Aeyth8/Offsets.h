#pragma once

class OFFSET;

namespace A8CL
{
namespace OFF
{
	// Basic UE Functions

	extern OFFSET FMalloc;
	extern OFFSET FRealloc;
	extern OFFSET FFree;
	extern OFFSET FTrim;

	extern OFFSET AppPreExit;

	extern OFFSET Browse;
	extern OFFSET UConsole;
	extern OFFSET ConsoleCommand;
	extern OFFSET PreLogin;
	extern OFFSET Login;
	
	extern OFFSET CopyString;

	// Native Game Functions

	extern OFFSET Import;
	extern OFFSET DebugImport;
	extern OFFSET EditorConstructor;
	extern OFFSET SetParentMap;
	extern OFFSET MapBP;
}
}