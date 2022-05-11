#include "stdafx.h"
#include "Mission1_Rocket.h"
#include "LineEdit/LineEdit.h"
#include "Objects/ColliderManager.h"

Rocket::Rocket(float x2, float y2,wstring directory,Vector2 scale, ColliderManager* colliderManager)
	:colliderManager(colliderManager)
{
	deafault = new Sprite(directory + L"Misson_1_Rocket_deafault.png", scale);
	deafault->SetPosition((deafault->GetTextureSize().x + 1328)*scale.x - x2, (deafault->GetTextureSize().y + 32)*scale.y - y2);
	deafault_hit = new Sprite(directory + L"Misson_1_Rocket_deafault_hit.png", scale);
	deafault_hit->SetPosition((deafault_hit->GetTextureSize().x + 1620)*scale.x - x2, (deafault_hit->GetTextureSize().y + 178)*scale.y - y2);
	deafault_hit->Pass(2);
	deafault_cover = new Sprite(directory + L"Misson_1_Rocket_deafault_cover.png", scale);
	deafault_cover->SetPosition((deafault_cover->GetTextureSize().x + 1650)*scale.x - x2, (deafault_cover->GetTextureSize().y + 178)*scale.y - y2);
	broken = new Sprite(directory + L"Misson_1_Rocket_broken_back.png", scale);
	broken->SetPosition((broken->GetTextureSize().x + 1248)*scale.x - x2, (broken->GetTextureSize().y + 32)*scale.y - y2);
	broken_cover = new Sprite(directory + L"Misson_1_Rocket_broken_cover.png", scale);
	broken_cover->SetPosition((broken_cover->GetTextureSize().x + 1296)*scale.x - x2, (broken_cover->GetTextureSize().y + 32)*scale.y - y2);
	
	//È÷Æ®ÆÇÁ¤
	hitColldier = new Collider(Vector2((deafault_hit->GetTextureSize().x + 1620)*scale.x - x2, (deafault_hit->GetTextureSize().y + 178)*scale.y - y2),Vector2(80.0f,100.0f),scale);
	//ºí·ÎÅ·º¼·ý
	blockingVolume = new Collider(Vector2((deafault_hit->GetTextureSize().x + 1620)*scale.x - x2, (deafault_hit->GetTextureSize().y + 178)*scale.y - y2), Vector2(80.0f, 100.0f), scale);
	blockingVolume->SetOffset(-47.0f, -7.0f);
	colliderManager->AddBlockings(blockingVolume);

	teamNumb = 1;
	health_Current = 200.0f;
	health_Max = 200.0f;
	colliderManager->AddHitObjects((IDamage*)this);

	//¶¥
	ground_Line = new Line();
	ground_LineEdit = new LineEdit(ground_Line, "Mission1_Ground_Rocket");

	colliderManager->AddGrounds(ground_Line);
	ground_LineEdit->LoadFile();
}

Rocket::~Rocket()
{
	SafeDelete(deafault);
	SafeDelete(deafault_hit);
	SafeDelete(deafault_cover);
	SafeDelete(broken);
	SafeDelete(broken_cover);
	SafeDelete(ground_Line);
	SafeDelete(ground_LineEdit);
	SafeDelete(hitColldier);
}

void Rocket::Update()
{
	ground_Line->Update();
	ground_LineEdit->Update();
	hitColldier->Update();
	blockingVolume->Update();
	deafault->Update();
	deafault_hit->Update();
	deafault_cover->Update();
	broken->Update();
	broken_cover->Update();
}

void Rocket::RenderCover()
{
	if(Damaged)
	{
		broken_cover->Render();
		return;
	}
	ground_Line->Render();
	hitColldier->Render();
	blockingVolume->Render();
	deafault_hit->Render();
	deafault_cover->Render();
}

void Rocket::RenderBackground()
{
	if (Damaged)
	{
		broken->Render();
		return;
	}
	deafault->Render();
}
void Rocket::TakeDamage(float damage)
{
	health_Current -= damage;
	deafault_hit->SetLerpTime(0.2f);
	if (CheckDead())
	{
		Damaged = 1;
		colliderManager->DeleteHitObjetcs((IDamage*)this);
		colliderManager->DeleteGrounds(ground_Line);
		colliderManager->DeleteBlockings(blockingVolume);
		damagedEvent();
	}
}

bool Rocket::CheckDead()
{
	if (health_Current <= 0.0f)return 1;
	return false;
}

Collider * Rocket::GetHitCollider()
{
	return hitColldier;
}

void Rocket::AddEvent(function<void()> func)
{
	damagedEvent = func;
}
