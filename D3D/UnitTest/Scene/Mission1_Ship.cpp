#include "stdafx.h"
#include "Mission1_Ship.h"
#include "LineEdit/LineEdit.h"
#include "Objects/ColliderManager.h"

Ship::Ship(float x2, float y2, wstring directory, Vector2 scale, ColliderManager* colliderManager)
{
	back = new Animation(directory + L"Misson_1_Ship_Back_Closed.png");
	back->AddFrameData(0, 0, 0, 1, 1);
	back->AddSprite(directory + L"Misson_1_Ship_Back_Open.png");
	back->ReadFrameDatas(1);
	back->SetScale(scale);
	back->SetSheetOffset(1, 9.0f, 0.0f);
	back->SetPosition((back->GetTextureSize(0).x + 2816)*scale.x - x2, (back->GetTextureSize(0).y + 32)*scale.y - y2);

	door = new Animation(directory + L"Misson_1_Ship_door_Closed.png");
	door->AddFrameData(0, 0, 0, 1, 1);
	door->AddSprite(directory + L"Misson_1_Ship_door_Open.png");
	door->ReadFrameDatas(1);
	door->SetScale(scale);
	door->SetSheetOffset(1, -4.0f, -8.0f);
	door->SetPosition((door->GetTextureSize(0).x + 3010)*scale.x - x2, (door->GetTextureSize(0).y + 124)*scale.y - y2);

	cover = new Sprite(directory + L"Misson_1_Ship_cover.png", scale);
	cover->SetPosition((cover->GetTextureSize().x + 2668)*scale.x - x2, (cover->GetTextureSize().y + 32)*scale.y - y2);

	//cover_End = new Sprite(directory + L"Misson_1_Ship_cover_End.png", scale);

	//¶¥
	ground_Line = new Line();
	ground_Line_Deck = new Line();
	ground_LineEdit = new LineEdit(ground_Line, "Mission1_Ground_Ship");
	ground_LineEdit_Deck = new LineEdit(ground_Line_Deck, "Mission1_Ground_Ship_Deck");

	colliderManager->AddGrounds(ground_Line);
	colliderManager->AddGrounds(ground_Line_Deck);
	ground_LineEdit->LoadFile();
	ground_LineEdit_Deck->LoadFile();


	x = (door->GetTextureSize(0).x + 3005)*scale.x - x2; y = (door->GetTextureSize(0).y + 149)*scale.y - y2;
	sx = scale.x; sy = scale.y;
	door->SetPosition(x + (a*sx), y + (b*sy));
}

Ship::~Ship()
{
	SafeDelete(back);
	SafeDelete(door);
	SafeDelete(cover);
	SafeDelete(ground_Line);
	SafeDelete(ground_LineEdit);
	SafeDelete(ground_Line_Deck);
	SafeDelete(ground_LineEdit_Deck);

	//SafeDelete(cover_End);
}

void Ship::Update()
{
	back->Update();
	door->Update();
	cover->Update();
	//cover_End->Update();
	//ImGui::SliderFloat("x", &a,-100,100);
	//ImGui::SliderFloat("y", &b,-50,50);
	if (ImGui::Button("PlayOnce"))
	{
		door->PlayOnce(1);
		back->PlayOnce(1);
	}
	if (ImGui::Button("Stop"))
	{
		door->PlayLoop(0);
		back->PlayLoop(0);
	}
	//door->SetPosition(x + (a*sx), y + (b*sy));
	ground_Line->Update();
	ground_LineEdit->Update();
	ground_Line_Deck->Update();
	ground_LineEdit_Deck->Update();

}

void Ship::RenderCover()
{
	/*if (EvenetEnd)
	{
		cover_End->Render();
		return;
	}*/
	ground_Line->Render();
	ground_Line_Deck->Render();
	cover->Render();
}

void Ship::RenderBackground()
{
	door->Render();
	back->Render();
	if (EvenetEnd)return;
}

void Ship::Event()
{
	EvenetEnd = 1;
	back->PlayOnce(1);
	door->PlayOnce(1);
}
