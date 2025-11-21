#pragma once
#include<Windows.h>
#include<string>
#include<TlHelp32.h>
#include"SDKGame.h"
LPCSTR TargetProcess = "AndroidEmulatorEx.exe";
bool ShowMenu = true;
bool ImGui_Initialised = false;
bool CreateConsole = true;
bool EnableESP = false;

namespace Process {
	DWORD ID;
	HANDLE Handle;
	HWND Hwnd;
	WNDPROC WndProc;
	int WindowWidth;
	int WindowHeight;
	int WindowLeft;
	int WindowRight;
	int WindowTop;
	int WindowBottom;
	LPCSTR Title;
	LPCSTR ClassName;
	LPCSTR Path;
	HANDLE GameHandle;
	INT Width;
	INT Height;
}

namespace Data
{
	DWORD Ue4Base;
	DWORD Gworld;
	DWORD Gnames;
	DWORD Uworld;
	DWORD ViewMatrixBase;
	DWORD PersistentLevel;
	DWORD NetDriver;
	DWORD LocalPlayer;
	DWORD RootComponent;
	DWORD ServerConnection;
	DWORD PlayerController;
	DWORD LocalPlayerAddr;
	INT MyTeamID;
	DWORD MyLocalRoot;
	DWORD PlayerCameraManager;
	std::vector<DWORD> EntityList;


}
namespace Setting 
{
	bool ESP = false;
	bool Sekeleton = false;
	bool Box = false;
	bool Health = false;
	bool pName = false;
	bool Distance = false;
	bool LGBTMode = false;
}

namespace OverlayWindow {
	WNDCLASSEX WindowClass;
	HWND Hwnd;
	LPCSTR Name;
}

