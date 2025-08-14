#pragma once

/*

Written by Aeyth8

https://github.com/Aeyth8

Copyright (C) 2025 Aeyth8

*/

struct IDXGIFactory;

class DirectXHook
{
public:

	//inline static bool Ready{false};
	static void HookSwapChain(const IDXGIFactory* Factory);



};