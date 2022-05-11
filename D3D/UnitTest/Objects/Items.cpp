#include "stdafx.h"
#include "Items.h"
#include "ColliderManager.h"

ItemObj::ItemObj(ColliderManager* manager, Vector2 position, Vector2 scale, Vector2 offset)
	: manager(manager), position(position), scale(scale), offset(offset)
{

}

ItemObj::~ItemObj()
{
	SafeDelete(ani);
	SafeDelete(collider);
}

void ItemObj::Update()
{
	if (!activate)return;
	PosUpdate();
	AnimUpdate();
}

void ItemObj::Render()
{
	if (!activate)return;
	collider->Render();
	ani->Render();
}

void ItemObj::SetPosition(Vector2 position)
{
	this->position = position;
	Vector2 temp = position;
	temp += Math::mul(offset, scale);
	ani->SetPosition(temp);
	collider->SetPosition(position);
}

void ItemObj::SetPosition(float x, float y)
{
	SetPosition(Vector2(x, y));
}

void ItemObj::SetScale(Vector2 scale)
{
	this->scale = scale;
	ani->SetScale(scale);
	collider->SetScale(scale);
}

void ItemObj::SetScale(float x, float y)
{
	SetScale(Vector2(x, y));
}

void ItemObj::SetRotation(Vector3 rotation)
{
	ani->SetRotationDegree(rotation);
	collider->SetRotationDegree(rotation);
}

void ItemObj::SetRotation(float x, float y, float z)
{
	SetRotation(Vector3(x, y, z));
}

void ItemObj::Activate()
{
	activate = 1;
}

void ItemObj::Deactivate()
{
	activate = 0;
}