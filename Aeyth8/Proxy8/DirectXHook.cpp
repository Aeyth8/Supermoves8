#include "DirectXHook.h"
#include "../Global.hpp"
#include "../Hooks/Hooks.hpp"
#include <dxgi.h>

/*

Written by Aeyth8

https://github.com/Aeyth8

Copyright (C) 2025 Aeyth8

*/


A8CL::OFFSET SwapChain("CreateSwapChain", 0x0);
A8CL::OFFSET oPresent("Present", 0x0);


typedef HRESULT(__stdcall* Present_T)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* CreateSwapChain_T)(IDXGIFactory* pFactory, _In_ IUnknown* pDevice, _In_ DXGI_SWAP_CHAIN_DESC* pDesc, _Out_ IDXGISwapChain** ppSwapChain);

/*enum IDXGISwapchainVT : unsigned char
{
	// IUnknown

	QueryInterface = 0,
	AddRef = 1,
	Release = 2,

	// IDXGIObject

	SetPrivateData = 3,
	SetPrivateDataInterface = 4,
	GetPrivateData = 5,
	GetParent = 6,

	// IDXGIDeviceSubObject

	GetDevice = 7,

	// IDXGISwapChain

	Present = 8,
	GetBuffer = 9,
	SetFullscreenState = 10,
	GetFullscreenState = 11,
	GetDesc = 12,
	ResizeBuffers = 13,
	ResizeTarget = 14,
	GetContainingOutput = 15,
	GetFrameStatistics = 16,
	GetLastPresentCount = 17
};*/




static IUnknown* TheDevice{nullptr};

/*

I am going to have to hook numerous things if I want it to work with (only) DirectX, CreateDXGIFactory -> CreateSwapChain -> Present
I also have to hook CreateSwapChainForHwnd, for the CommandQueue thing

*/

static HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static bool Initialized = false;
	if (!Initialized)
	{
		Initialized = true;


	}

	return oPresent.VerifyFC<Present_T>()(pSwapChain, SyncInterval, Flags);
}



static HRESULT __stdcall CreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain)
{
	HRESULT Result = SwapChain.VerifyFC<CreateSwapChain_T>()(pFactory, pDevice, pDesc, ppSwapChain);

	static bool bPresent{false};
	if (!bPresent)
	{
		bPresent = true;

		uintptr_t* VFTable = *(uintptr_t**)*ppSwapChain;
		oPresent.Address = VFTable[0x8];

		TheDevice = pDevice;

		A8CL::Hooks::CreateAndEnableHook(oPresent, Present);
	}
	A8CL::Global::LogA("CreateSwapChain", "HJELPPPPPPPPPPPPPP HELP MEEEEEEEEEEEEEEEEEEEEEEEEEE");

	return Result;
}







void DirectXHook::HookSwapChain(const IDXGIFactory* Factory)
{
	static bool IsActivated{false};

	uintptr_t* VFTable = *(uintptr_t**)Factory;
	SwapChain.Address = VFTable[0xA];
	A8CL::Global::LogA("CreateSwapChain", A8CL::Global::HexToString(SwapChain.Address) + " | Byte : " + A8CL::Global::HexToString(A8CL::Global::OffsetToByte(SwapChain.Address)));
	if (A8CL::Global::OffsetToByte(SwapChain.Address) == 0xE9)
	{
		return;
		uintptr_t JumpTo = (SwapChain.Address + 5) + *(int32_t*)(SwapChain.Address + 1);
		A8CL::Global::LogA("SwapChainJump", A8CL::Global::HexToString(JumpTo) + " | Byte : " + A8CL::Global::HexToString(A8CL::Global::OffsetToByte(JumpTo)));
		if (A8CL::Global::OffsetToByte(JumpTo) == 0xE9) return;
	}

	if (!IsActivated)
	{
		IsActivated = true;

		

		

		if (A8CL::Hooks::Init())
		{
			A8CL::Hooks::CreateAndEnableHook(SwapChain, CreateSwapChain);
		}
		
	}
}