// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include "mem.h"
#include "game.h"


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

	bool freezeTimer = false, displayMines = false, tilesInert = false;

	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}

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

		}

		if (GetAsyncKeyState(VK_F2) & 1)
		{
			displayMines = !displayMines;

			if (displayMines)
			{


				//BYTE grid[27][32];
				BYTE* array = (BYTE*)(0x01005340);

				BYTE* height = (BYTE*)0x01005338;
				BYTE* width = (BYTE*)0x01005334;
				unsigned int rows = *(height)+2;
				unsigned int cols = *(width)+2;
				std::vector<std::vector<BYTE> > grid(27, std::vector<BYTE>(32));

				game::fillArray(array, grid, rows, cols);
				game::displayGrid(grid, rows, cols);

			}

		}

		if (GetAsyncKeyState(VK_F3) & 1)
		{
			tilesInert = !tilesInert;
		}

		Sleep(5);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
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