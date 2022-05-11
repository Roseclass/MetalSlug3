#include "stdafx.h"
#include "Items_HeavyMG.h"
#include "ColliderManager.h"
#include "Characters/Player_Eri.h"

Items_HeavyMG::Items_HeavyMG(ColliderManager * manager, Vector2 position, Vector2 scale, Vector2 offset)
	:ItemObj(manager, position, scale, offset)
{
	Init();
	itemNumb = 1;
	manager->AddItems((ItemObj*)this);
	SetPosition(position);
	SetScale(scale);
}

Items_HeavyMG::~Items_HeavyMG()
{

}

void Items_HeavyMG::Init()
{
	AnimInit();
	ColliderInit();
}

void Items_HeavyMG::AnimInit()
{
	ani = new Animation();
	ani->AddSprite(L"misc/Items_HeavyMG.png");
	ani->AddFrameData(Vector2(0.0f, 0.0f), Vector2(22.0f, 20.0f));
}

void Items_HeavyMG::ColliderInit()
{
	collider = new Collider(position, Vector2(22.0f, 20.0f), scale);
	collider->SetOffset(offset);
}

void Items_HeavyMG::PosUpdate()
{
	Vector2 pos = position;
	if (manager->IntersectGrounds(&pos)) velocity = 0.0f;
	else
	{
		velocity -= 0.98f;
		pos.y += velocity * Time::Delta();
	}
	SetPosition(pos);
}

void Items_HeavyMG::AnimUpdate()
{
	ani->Update();
	collider->Update();
}
