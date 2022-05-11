#include "stdafx.h"
#include "Main.h"
#include "SelectScene.h"
#include "Mission1.h"

void SelectScene::Initialize()
{
	wstring directory= L"/background/Select/";
	///배경
	background = new Sprite(directory+L"Select_Back.png");

	///글로벌스케일 설정
	scale.x = Context::Get()->GetViewport()->GetWidth()/background->GetTexture()->GetWidth()*0.5f;
	scale.y = Context::Get()->GetViewport()->GetHeight()/background->GetTexture()->GetHeight()*0.5f;
	float offsetX =Context::Get()->GetViewport()->GetWidth()*0.5;
	background->SetScale(scale);
	///커버
	background_Cover = new Sprite(directory+L"Select_Back_Cover.png", scale);
	///문
	doors[0] = new Door(directory, 0, 102 * scale.x - offsetX, 222 * scale.y, -44 * scale.y, scale);
	doors[1] = new Door(directory, 1, 238 * scale.x - offsetX, 222 * scale.y, -44 * scale.y, scale);
	doors[2] = new Door(directory, 2, 374 * scale.x - offsetX, 222 * scale.y, -44 * scale.y, scale);
	doors[3] = new Door(directory, 3, 510 * scale.x - offsetX, 222 * scale.y, -44 * scale.y, scale);
	///사인
	prevTime = Time::Get()->Running();
	for (int i = 0; i < 4; i++)
	{
		sign[i][0] = new Sprite(directory + L"Select_Sign" + to_wstring(i + 1) + L"_01.png", scale);
		sign[i][1] = new Sprite(directory + L"Select_Sign" + to_wstring(i + 1) + L"_02.png", scale);
	}
	sign[0][0]->SetPosition(96 * scale.x - offsetX, 112 * scale.y);
	sign[0][1]->SetPosition(96 * scale.x - offsetX, 112 * scale.y);
	sign[1][0]->SetPosition(224 * scale.x - offsetX, 112 * scale.y);
	sign[1][1]->SetPosition(224 * scale.x - offsetX, 112 * scale.y);
	sign[2][0]->SetPosition(384 * scale.x - offsetX, 112 * scale.y);
	sign[2][1]->SetPosition(384 * scale.x - offsetX, 112 * scale.y);
	sign[3][0]->SetPosition(512 * scale.x - offsetX, 112 * scale.y);
	sign[3][1]->SetPosition(512 * scale.x - offsetX, 112 * scale.y);
	///초상화
	for (int i = 1; i <= 3; i++)
	{
		marco[i - 1] = new Sprite(directory + L"Select_Marco_0" + to_wstring(i) + L".png", scale);
		marco[i - 1]->SetPosition(102 * scale.x - offsetX, -41 * scale.y);
		eri[i - 1] = new Sprite(directory + L"Select_Eri_0" + to_wstring(i) + L".png", scale);
		eri[i - 1]->SetPosition(238 * scale.x - offsetX, -41 * scale.y);
		tarma[i - 1] = new Sprite(directory + L"Select_Tarma_0" + to_wstring(i) + L".png", scale);
		tarma[i - 1]->SetPosition(374 * scale.x - offsetX, -41 * scale.y);
		fio[i - 1] = new Sprite(directory + L"Select_Fio_0" + to_wstring(i) + L".png", scale);
		fio[i - 1]->SetPosition(510 * scale.x - offsetX, -41 * scale.y);
	}
	marco[1]->Pass(1);
	eri[1]->Pass(1);
	tarma[1]->Pass(1);
	fio[1]->Pass(1);
	main->LoadMission1();
}

void SelectScene::Destroy()
{
	SafeDelete(background);
	SafeDelete(background_Cover);
	for (int i = 0; i < 3; i++)
	{
		SafeDelete(eri[i]);
		SafeDelete(fio[i]);
		SafeDelete(marco[i]);
		SafeDelete(tarma[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		SafeDelete(doors[i]);
		SafeDelete(sign[i][0]);
		SafeDelete(sign[i][1]);
	}
}

void SelectScene::Update()
{
	///사인점멸
	float curTime = Time::Get()->Running();
	if (curTime - prevTime >= 1.0f/10.0f)
	{
		signNumb = (signNumb + 1) % 2;
		prevTime = curTime;
	}

	background->Update();
	background_Cover->Update();
	for (int i = 0; i < 3; i++)
	{
		marco[i]->Update();
		eri[i]->Update();
		tarma[i]->Update();
		fio[i]->Update();
	}
	for (int i = 0; i < 4; i++)
	{
		sign[i][0]->Update();
		sign[i][1]->Update();
	}
	for (int i = 0; i < 4; i++)
		doors[i]->Update();
	if (!open)
	{
		for (int i = 0; i < 4; i++)
		{
			if (doors[i]->Open() == false)	break;
			if (i == 3)open = 1;
		}
	}

	lerpTime += 0.2*Time::Delta();
	if (lerpTime >= 0.75f) lerpTime = 0.75f;
	if (Time::Get()->Running() - selectTime > 3.0f &&select)
	{
		LoadNextScene();
	}
	if (select|| !open) return;

	///테스트
	///static float x = 0, y = 0;
	///ImGui::SliderFloat("posX", &x, 0.0f, 700.0f);
	///ImGui::SliderFloat("posY", &y, -250.0f, 500.0f);

	if (Keyboard::Get()->Down(VK_LEFT))
	{
		selectNumb--;
		lerpTime = 0.1f;
	}
	else if (Keyboard::Get()->Down(VK_RIGHT))
	{
		selectNumb++;
		lerpTime = 0.1f;
	}
	if (selectNumb < 0) selectNumb = 3;
	selectNumb %= 4;

	if (Keyboard::Get()->Down('X'))
	{
		select = 1;
		selectTime = Time::Get()->Running();
		CloseDoor();
	}
}

void SelectScene::Render()
{
	if (open)sign[selectNumb][signNumb]->Render();
	background_Cover->Render();

	doors[0]->Render();
	doors[1]->Render();
	doors[2]->Render();
	doors[3]->Render();

	if (selectNumb == 0 && open) 
	{
		marco[1]->SetLerpTime(lerpTime);
		marco[1 + select]->Render();
	}else marco[0]->Render();
	if (selectNumb == 1 && open) 
	{ 
		eri[1]->SetLerpTime(lerpTime);
		eri[1 + select]->Render();
	}else eri[0]->Render();
	if (selectNumb == 2 && open)
	{
		tarma[1]->SetLerpTime(lerpTime);
		tarma[1 + select]->Render();
	}else tarma[0]->Render();
	if (selectNumb == 3 && open)
	{
		fio[1]->SetLerpTime(lerpTime);
		fio[1 + select]->Render();
	}else fio[0]->Render();

	background->Render();
}

void SelectScene::CloseDoor()
{
	doors[selectNumb]->NextSequence();
}

void SelectScene::LoadNextScene()
{
	main->NextScene();
}
