#include "stdafx.h"
#include <iostream>
#include "mem.h"
#include "game.h"

void displayMenu();
typedef void(__stdcall* _winFun)(int param1);
typedef void(__stdcall* _displayMinesFun)(int param1);
_winFun winFun;
_displayMinesFun displayMines;

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "Minesweeper Trainer\n";

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"winmine.exe");

	//calling it with NULL also gives you the address of the .exe module
	moduleBase = (uintptr_t)GetModuleHandle(NULL);

	bool freezeTimer = false, displayGrid = false, tilesInert = false;

	displayMenu();
	
	while (true)
	{
		// Quit
		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}

		// Freeze timer
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			freezeTimer = !freezeTimer;

			if (freezeTimer)
			{
				std::cout << "Freeze timer [TRUE]\n";
				mem::Nop((BYTE*)(moduleBase + 0x2ff5), 6);
			}
			else
			{
				std::cout << "Freeze timer [FALSE]\n";
				mem::Patch((BYTE*)(moduleBase + 0x2ff5), (BYTE*)"\xFF\x05\x9C\x57\x00\x01", 6);
			}

			displayMenu();
		}

		// Display Grid
		if (GetAsyncKeyState(VK_F2) & 1)
		{
			BYTE* array = (BYTE*)(0x01005340);

			DWORD32* height = (DWORD32*)0x01005338;
			DWORD32* width = (DWORD32*)0x01005334;
			unsigned int rows = *(height)+2;
			unsigned int cols = *(width)+2;
			std::vector<std::vector<BYTE> > grid(27, std::vector<BYTE>(32));

			game::fillArray(array, grid, rows, cols);
			game::displayGrid(grid, rows, cols);

			displayMenu();
		}

		if (GetAsyncKeyState(VK_F3) & 1)
		{
			tilesInert = !tilesInert;
			displayMenu();
		}

		if (GetAsyncKeyState(VK_F4) & 1)
		{
			winFun = (_winFun)(moduleBase + 0x347C);
			winFun(1);
			displayMenu();
		}

		if (GetAsyncKeyState(VK_F5))
		{
			displayMines = (_displayMinesFun)(moduleBase + 0x2f80);
			displayMines(0xA);
			displayMenu();
		}

		Sleep(5);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

void displayMenu()
{
	std::string menu = "[F1] Freeze Timer\n[F2] Display Grid\n[F3] Set Tiles Inert\n[F4] Auto Win\n[F5] Display Mines In Game\n[END] Exit\n\n";
	std::cout << menu;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}