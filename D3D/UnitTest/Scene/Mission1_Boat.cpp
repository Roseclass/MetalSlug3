#include "stdafx.h"
#include "Mission1_Boat.h"
#include "LineEdit/LineEdit.h"
#include "Objects/ColliderManager.h"

Boat::Boat(float x2, float y2, wstring directory, Vector2 scale, ColliderManager* colliderManager)
	:colliderManager(colliderManager)
{
	deafault = new Sprite(directory + L"Misson_1_Boat_deafault.png", scale);
	deafault->SetPosition((deafault->GetTextureSize().x + 1844)*scale.x - x2, (deafault->GetTextureSize().y + 32)*scale.y - y2);
	broken = new Sprite(directory + L"Misson_1_Boat_back.png", scale);
	broken->SetPosition((broken->GetTextureSize().x + 1844)*scale.x - x2, (broken->GetTextureSize().y + 32)*scale.y - y2);
	broken_cover = new Sprite(directory + L"Misson_1_Boat_cover.png", scale);
	broken_cover->SetPosition((broken_cover->GetTextureSize().x + 1844)*scale.x - x2, (broken_cover->GetTextureSize().y + 32)*scale.y - y2);

	//È÷Æ®ÆÇÁ¤
	hitColldier = new Collider(Vector2((deafault->GetTextureSize().x + 1844)*scale.x - x2, (deafault->GetTextureSize().y + 32)*scale.y - y2), Vector2(123.0f, 37.0f), scale);
	hitColldier->SetOffset(-31.0f, 0.0f);

	//ºí·ÎÅ· º¼·ý
	blockingVolume = new Collider(Vector2((deafault->GetTextureSize().x + 1844)*scale.x - x2, (deafault->GetTextureSize().y + 32)*scale.y - y2), Vector2(100.0f, 35.0f), scale);
	blockingVolume->SetOffset(-45.0f, 0.0f);
	colliderManager->AddBlockings(blockingVolume);

	teamNumb = 1;
	health_Current = 200.0f;
	health_Max = 200.0f;
	colliderManager->AddHitObjects((IDamage*)this);

	//¶¥
	ground_Line = new Line();
	ground_LineEdit = new LineEdit(ground_Line, "Mission1_Ground_Boat");

	colliderManager->AddGrounds(ground_Line);
	ground_LineEdit->LoadFile();
}

Boat::~Boat()
{
	SafeDelete(deafault);
	SafeDelete(broken);
	SafeDelete(broken_cover);
	SafeDelete(ground_Line);
	SafeDelete(ground_LineEdit);
	SafeDelete(hitColldier);
	SafeDelete(blockingVolume);
}

void Boat::Update()
{
	//static float Boatx = 0.0f, Boaty = 0.0f;
	//static float Boatsx = 0.0f, Boatsy = 0.0f;
	//ImGui::Begin("Boat");
	//ImGui::SliderFloat("offsetX", &Boatx, -100.0f, 100.0f);
	//ImGui::SliderFloat("offsetY", &Boaty, -100.0f, 100.0f);
	//ImGui::SliderFloat("sizeX", &Boatsx, 0.0f, 200.0f);
	//ImGui::SliderFloat("sizeY", &Boatsy, 0.0f, 200.0f);
	//hitColldier->SetSize(Boatsx, Boatsy);
	//hitColldier->SetOffset(Boatx, Boaty);
	//ImGui::End();

	ground_Line->Update();
	ground_LineEdit->Update();
	deafault->Update();
	broken->Update();
	broken_cover->Update();
	hitColldier->Update();
	blockingVolume->Update();
}

void Boat::RenderCover()
{
	if (Damaged)broken_cover->Render();
	else
	{
		ground_Line->Render();
		hitColldier->Render();
		blockingVolume->Render();
	}
}

void Boat::RenderBackground()
{
	if (Damaged)
	{
		broken->Render();
		return;
	}
	deafault->Render();
}

void Boat::TakeDamage(float damage)
{
	health_Current -= damage;
	if (CheckDead())
	{
		Damaged = 1;
		colliderManager->DeleteHitObjetcs((IDamage*)this);
		colliderManager->DeleteGrounds(ground_Line);
		colliderManager->DeleteBlockings(blockingVolume);
		damagedEvent();
	}
}

bool Boat::CheckDead()
{
	if (health_Current <= 0.0f)return 1;
	return false;
}

Collider* Boat::GetHitCollider()
{
	return hitColldier;
}

void Boat::AddEvent(function<void()> func)
{
	damagedEvent = func;
}
