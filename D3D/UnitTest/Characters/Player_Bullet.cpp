#include "stdafx.h"
#include "Player_Bullet.h"
#include "Objects/ColliderManager.h"
Bullet_Pistol::Bullet_Pistol(UINT teamNumber, UINT poolIndex, float damage, ProjectileManager * manager, ColliderManager* colliderManager)
	: Bullet(teamNumber, poolIndex, damage, manager, colliderManager)
{
	collider = new Collider(Vector2(0.0f, 0.0f), Vector2(12.0f, 4.0f));
	collider->SetOffset(0, 0);
	ani = new Animation();
	ani->AddSprite(L"character/Weapon_Pistol_Bullet.png");
	ani->AddFrameData(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
	ani->SetScale(0.5f, 0.5f);
	SetTeamNumber(0);
	debug = 1;
}

Bullet_Pistol::~Bullet_Pistol()
{
}

void Bullet_Pistol::Update()
{
	if (!activated) return;
	CollisionCheck();
	if (Time::Get()->Running() - activationTime >= lifespan)
	{
		Deactivate();
		activationTime = Time::Get()->Running();
	}
	if (ani->IsPlay() == 0)
	{
		Deactivate();
		return;
	}
	Progress();
	Bullet::Update();
}

void Bullet_Pistol::Render()
{
	if (!activated) return;
	Bullet::Render();
	ani->Render();
}

void Bullet_Pistol::Progress()
{
	if (!move)return;
	float speed = Time::Delta()*1000.0f;
	Vector3 temp = ani->GetPosition();
	temp += speed * forward;
	ani->SetPosition(temp.x, temp.y);
	collider->SetPosition(temp);
}

void Bullet_Pistol::Activate(float z)
{		
	activationTime = Time::Get()->Running();
	forward.x = cos(z*Radian);
	forward.y = sin(z*Radian);
	activated = 1;
	move = 1;
}

void Bullet_Pistol::Deactivate()
{
	activationTime = 0.0f;
	activated = 0;
}

void Bullet_Pistol::CollisionCheck()
{
	colliderManager->IntersectObjects((Bullet*)this);
	colliderManager->IntersectGrounds((Bullet*)this);
}

void Bullet_Pistol::Pop()
{
	Deactivate();
}
////////////////////////////////////////////Bullet_HeavyMG////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Bullet_HeavyMG::Bullet_HeavyMG(UINT teamNumber, UINT poolIndex, float damage, ProjectileManager * manager, ColliderManager * colliderManager)
	: Bullet(teamNumber, poolIndex, damage, manager, colliderManager)
{
	collider = new Collider(Vector2(0.0f, 0.0f), Vector2(35.0f, 4.0f));
	collider->SetOffset(0.0f, 0.0f);
	ani = new Animation();
	ani->AddSprite(L"character/Weapon_Heavy_Bullet.png");
	ani->AddFrameData(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
	SetTeamNumber(0);
	debug = 1;
}

Bullet_HeavyMG::~Bullet_HeavyMG()
{
}

void Bullet_HeavyMG::Update()
{
	if (!activated) return;
	CollisionCheck();
	if (Time::Get()->Running() - activationTime >= lifespan)
	{
		Deactivate();
		activationTime = Time::Get()->Running();
	}
	if (ani->IsPlay() == 0)
	{
		Deactivate();
		return;
	}
	Progress();
	Bullet::Update();
}

void Bullet_HeavyMG::Render()
{
	if (!activated) return;
	Bullet::Render();
	ani->Render();
}

void Bullet_HeavyMG::Progress()
{
	if (!move)return;
	float speed = Time::Delta()*1000.0f;
	Vector3 temp = ani->GetPosition();
	temp += speed * forward;
	ani->SetPosition(temp.x, temp.y);
	collider->SetPosition(temp);
}

void Bullet_HeavyMG::Activate(float z)
{
	activationTime = Time::Get()->Running();
	forward.x = cos(z*Radian);
	forward.y = sin(z*Radian);
	activated = 1;
	move = 1;
}

void Bullet_HeavyMG::Deactivate()
{
	activationTime = 0.0f;
	activated = 0;
}

void Bullet_HeavyMG::CollisionCheck()
{
	colliderManager->IntersectObjects((Bullet*)this);
	colliderManager->IntersectGrounds((Bullet*)this);
}

void Bullet_HeavyMG::Pop()
{
	Deactivate();
}
