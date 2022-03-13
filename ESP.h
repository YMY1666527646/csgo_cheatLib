#pragma once
#include "GDI.h"
#include "GameSpecific.h"

void DrawPlayerBox(BasePlayer* player)
{
	int team = player->TeamNum();
	int health = player->Health();

	Vector3 pos = player->getPos();
	Vector3 head;
	head.x = pos.x;
	head.y = pos.y;
	head.z = pos.z + 75.f;
	Vector3 screenpos = WorldToScreen(pos, gameVars::vm);
	Vector3 screenhead = WorldToScreen(head, gameVars::vm);
	float height = screenhead.y - screenpos.y;
	float width = height / 2.4f;

	if (screenpos.z >= 0.01f && health > 0) {
		DrawBorderBox(screenpos.x - (width / 2), screenpos.y, width, height, 1);
		DrawLine(GDI::screenX / 2, GDI::screenY, screenpos.x, screenpos.y);
	}
}