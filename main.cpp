#include <iostream>
#include "ProcessManager.h"
#include <Windows.h>
#include "Offsets.h"
#include "ProcessMemory.h"
#include "GDI.h"
#include "GameSpecific.h"
#include "ESP.h"
#include <string>

void INIT()
{
	gameVars::MAX_PLAYERS = RPM<int>(RPM<uintptr_t>(process::engineBase + signatures::dwClientState) + signatures::dwClientState_MaxPlayer);
}

int main()
{
	process::procId = GetProcId(L"csgo.exe");
	process::moduleBase = GetModuleBaseAddress(process::procId, L"client.dll");
	process::hProc = OpenProcess(PROCESS_VM_READ, NULL, process::procId);
	process::engineBase = GetModuleBaseAddress(process::procId, L"engine.dll");

	std::cout << "PROCESS ID:\t" << process::procId << std::endl << std::hex << "moduleBase Address client.dll:\t" << process::moduleBase << std::endl <<
		"PROCESS HANDLE:\t" << process::hProc << std::endl << std::endl << std::endl;

	BasePlayer* LPlayer = LocalPlayer();

	INIT();
	std::vector<BasePlayer*> players = PlayerList();

	std::cout << GDI::screenX << " " << GDI::screenY << std::endl;

	std::string toOut = "";
	while (true)
	{
		std::cout << toOut;
		toOut = "";
		for (BasePlayer* player : players)
		{
			toOut += std::to_string(LPlayer->getDistance(player)) + " - distance to Player with heatlh:" + std::to_string(player->Health()) + "\n";
		}
		system("cls");
	}
}