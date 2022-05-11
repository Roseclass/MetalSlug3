#include "stdafx.h"
#include "TutorialScene.h"

void TutorialScene::Initialize()
{
	directory= L"/background/How_to_Play/";
	foreGround= new Sprite(directory+L"ForeGround.png");
	///글로벌 스케일 설정
	scale.x = Context::Get()->GetViewport()->GetWidth() / foreGround->GetTexture()->GetWidth()*0.5f;
	scale.y = Context::Get()->GetViewport()->GetHeight() / foreGround->GetTexture()->GetHeight()*0.5f;
	foreGround->SetScale(scale);
	///배경
	backGround=new Sprite(directory + L"BackGround.png", scale);
	backGround->SetPosition(0.0f,67.0f);
	backGround_Sky = new Sprite(directory + L"BackGround_Sky.png", scale);
	backGround_Sky->SetPosition(0.0f, 175.0f);
	///알파벳
	alphabet[0] = new Sprite(directory + L"Alphabet_A.png", scale);
	alphabet[1] = new Sprite(directory + L"Alphabet_B.png", scale);
	alphabet[2] = new Sprite(directory + L"Alphabet_C.png", scale);
	alphabet[3] = new Sprite(directory + L"Alphabet_D.png", scale);
	alphabet[4] = new Sprite(directory + L"Alphabet_X.png", scale);
	///조이스틱 커서
	cursor[0] = new Sprite(directory + L"Cursor_Horizontal.png", scale);
	cursor[0]->SetPosition(-400.0f, -270.0f);
	cursor[1] = new Sprite(directory + L"Cursor_Vertical.png", scale);
	cursor[1]->SetPosition(-400.0f, -270.0f);
	cursor[2] = new Sprite(directory + L"Cursor_Diagonal.png", scale);
	cursor[2]->SetPosition(-400.0f, -270.0f);
	///조이스틱
	joyStick = new Animation(directory + L"JoyStick.png");
	joyStick->SetScale(scale);
	joyStick->AddFrameData(0,3.0f, 0.0f, 39.0f, 52.0f, 0.0f, 0.0f);
	joyStick->AddFrameData(0,38.0f, 0.0f, 74.0f, 52.0f, 6.0f, 0.0f);
	joyStick->AddFrameData(0,73.0f, 0.0f, 109.0f, 52.0f, 12.0f, 0.0f);
	joyStick->AddFrameData(0,1.0f, 55.0f, 38.0f, 107.0f, -3.0f, 2.0f);
	joyStick->AddFrameData(0,37.0f, 55.0f, 74.0f, 107.0f, 5.0f, 2.0f);
	joyStick->AddFrameData(0,76.0f, 55.0f, 113.0f, 107.0f, 17.0f, 2.0f);
	joyStick->AddFrameData(0,2.0f, 108.0f, 39.0f, 160.0f, -1.0f, 2.0f);
	joyStick->AddFrameData(0,38.0f, 108.0f, 75.0f, 160.0f, 6.0f, 2.0f);
	joyStick->AddFrameData(0,75.0f, 108.0f, 112.0f, 160.0f, 15.0f, 2.0f);
	joyStick->SetPerSecond(0.001f);
	joyStick->SetPosition(-400.0f, -165.0f);
	///버튼
	buttons[0] = new Animation(directory + L"Button_Red.png");	
	buttons[1] = new Animation(directory + L"Button_Yellow.png");
	buttons[2] = new Animation(directory + L"Button_Green.png");
	buttons[3] = new Animation(directory + L"Button_Blue.png");
	for (int i = 0; i < 4; i++)
	{
		buttons[i]->SetScale(scale);
		buttons[i]->SetPerSecond(0.001f);
		buttons[i]->AddFrameData(0,0.0f, 0.0f, 38.0f, 50.0f, 0.0f, 0.0f);
		buttons[i]->AddFrameData(0,37.0f, 0.0f, 75.0f, 50.0f, 0.0f, 0.0f);
		buttons[i]->AddFrameData(0,73.0f, 0.0f, 111.0f, 50.0f, -2.0f, 0.0f);
		buttons[i]->SetFrameNumb(0);
		buttons[i]->SetPosition(-180.0f + i * 200.0f, -175.0f);
		alphabet[i]->SetPosition(-180.0f + i * 200.0f, -270.0f);
	}alphabet[4]->SetPosition(420.0f, -200.0f);
}

void TutorialScene::Destroy()
{
	SafeDelete(backGround);
	SafeDelete(backGround_Sky);
	SafeDelete(foreGround);
	SafeDelete(joyStick);
	for(int i =0; i<5; i++)
		SafeDelete(alphabet[i]);
	for(int i =0; i<3; i++)
		SafeDelete(cursor[i]);
	for (int i = 0; i < 4; i++)
		SafeDelete(buttons[i]);
}

void TutorialScene::Update()
{
	backGround->Update();
	backGround_Sky->Update();
	foreGround->Update();

	cursor[0]->Update();
	cursor[1]->Update();
	cursor[2]->Update();

	joyStick->SetFrameNumb(stickDirection);
	joyStick->Update();
	for (int i = 0; i < 4; i++)
		buttons[i]->Update();
	for (int i = 0; i < 5; i++)
		alphabet[i]->Update();

	static float pos[3];
	//ImGui::SliderFloat3("pos", pos, -400, 400);
	//alphabet[0]->SetPosition(pos[0], pos[1]);
	//ImGui::SliderInt("stickNumb", (int*)&stickDirection,0,8);
}

void TutorialScene::Render()
{
	joyStick->Render();
	SetCursorDirection();

	for (int i = 0; i < 5; i++)
		alphabet[i]->Render();
	for (int i = 0; i < 4; i++)
		buttons[i]->Render();

	foreGround->Render();
	backGround->Render();
	backGround_Sky->Render();
}

void TutorialScene::SetCursorDirection()
{	//0:horizon 1:vertical 2:diagonal
	//012
	//345
	//678
	switch (stickDirection)
	{
	case 0:cursor[2]->SetRotationDegree(180.0f, 180.0f, 0.0f); break;	//좌상
	case 1:cursor[1]->SetRotationDegree(180.0f, 0.0f, 0.0f); break;		//상
	case 2:cursor[2]->SetRotationDegree(180.0f, 0.0f, 0.0f); break;		//우상
	case 3:cursor[0]->SetRotationDegree(0.0f, 180.0f, 0.0f); break;		//좌
	case 4:return;
	case 5:cursor[0]->SetRotationDegree(0.0f, 0.0f, 0.0f); break;		//우
	case 6:cursor[2]->SetRotationDegree(0.0f, 180.0f, 0.0f); break;		//좌하
	case 7:cursor[1]->SetRotationDegree(0.0f, 0.0f, 0.0f); break;		//하
	case 8:cursor[2]->SetRotationDegree(0.0f, 0.0f, 0.0f); break;		//우하
	default:break;
	}
	if (stickDirection == 3 || stickDirection == 5) cursor[0]->Render();
	else if (stickDirection == 1 || stickDirection == 7) cursor[1]->Render();
	else cursor[2]->Render();
}
