#pragma once
#include <TlHelp32.h>
#include <Windows.h>
#include "ProcessMemory.h"
#include "Offsets.h"
#include <vector>
#include <cstdlib>
#include <math.h>

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};

namespace gameVars
{
	int MAX_PLAYERS;
	view_matrix_t vm;
};

struct Vector3
{
	float x, y, z;
};

Vector3 WorldToScreen(const Vector3 pos, view_matrix_t matrix) {
	float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = GDI::screenX * .5f;
	float y = GDI::screenY * .5f;

	x += 0.5f * _x * GDI::screenX + 0.5f;
	y -= 0.5f * _y * GDI::screenY + 0.5f;

	return { x,y,w };
}


class BasePlayer
{
public:
	int Health()
	{
		return RPM<int>((uintptr_t)this + player::m_iHealth);
	}

	int TeamNum()
	{
		return RPM<int>((uintptr_t)this + player::m_iTeamNum);
	}

	Vector3 getPos()
	{
		return RPM<Vector3>((uintptr_t)this + player::m_vecOrigin);
	}

	bool IsPlayer()
	{
		int index = RPM<int>((uintptr_t)this + player::m_dwPlayerIndex);
		return (index > 0 && index < gameVars::MAX_PLAYERS);
	}

	float getDistance(BasePlayer* player)
	{
		Vector3 distance;
		distance.x = std::abs(this->getPos().x - player->getPos().x);
		distance.y = std::abs(this->getPos().y - player->getPos().y);
		distance.z = std::abs(this->getPos().z - player->getPos().z);
		
		return sqrtf(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
	}
};

BasePlayer* LocalPlayer()
{
	std::cout << "\nGetting LocalPlayer...\nMemory To Read: ";
	std::cout << std::hex << (process::moduleBase + signatures::dwLocalPlayer);
	BasePlayer* lp = RPM<BasePlayer*>(process::moduleBase + signatures::dwLocalPlayer);
	std::cout << "\nReaded one: " << std::hex << (uintptr_t)lp << std::endl;
	return lp;
}

std::vector<BasePlayer*> PlayerList()	
{
	std::vector<BasePlayer*> players;
	for (int i = 0; i < gameVars::MAX_PLAYERS; i++)
	{
		BasePlayer* player = RPM<BasePlayer*>(process::moduleBase + signatures::dwEntityList + (i * 0x10));
		if (player->IsPlayer())
			players.push_back(player);
		else
			break;
	}
	return players;
}

