#pragma once
#include "Systems/IExecute.h"
class Door;
class SelectScene : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};
	virtual void SetMain(IExecute* main)override { this->main = dynamic_cast<Main*>(main); };
	virtual void Start()override {};

private:
	float lerpTime = 0.0f;
	int selectNumb=0;
	UINT signNumb=0;
	bool select = 0; float selectTime;
	bool open = 0;
	Sprite* background;
	Sprite* background_Cover;
	Door* doors[4];
	Sprite* eri[3];
	Sprite* fio[3];
	Sprite* marco[3];
	Sprite* tarma[3];
	Sprite* sign[4][2];
	Vector2 scale;	//글로벌 스케일
	float prevTime;	//사인 점멸용
	void CloseDoor();
private:
	Main* main;
	void LoadNextScene();
};

class Door
{
public:
	Door(wstring directory,UINT selectCount,float x, float top, float bottom, Vector2 scale = Vector2(1.0f, 1.0f), Vector2 start = Vector2(0, 0), Vector2 end = Vector2(1.0f, 1.0f))
	{
		this->directory = directory;
		image = new Sprite(directory +L"Select_Door.png", scale, start, end);
		capsule = new Sprite(directory +L"Select_Door_Capsule.png", scale, start, end);
		SetBound(x, top, bottom);
		SetPosition(x, bottom);
		speed = 1000;
		delay = (rand() % 30+1) / 60.0f;
		spark = new Animation(directory + L"Door_Effect.png");
		spark->SetScale(scale);
		spark->SetPerSecond(20.0f);
		spark->AddFrameData(0, 2.0f, 1.0f, 2.0f + 67.0f, 1.0f + 16.0f);
		spark->AddFrameData(0, 71.0f, 1.0f, 71.0f + 70.0f, 1.0f + 19.0f, -1.0f, -3.0f);
		spark->AddFrameData(0, 2.0f, 22.0f, 2.0f + 73.0f, 22.0f + 21.0f, -2.0f, -11.0f);
		spark->AddFrameData(0, 77.0f, 22.0f, 77.0f + 76.0f, 22.0f + 24.0f, -3.0f, -17.0f);
		spark->AddFrameData(0, 2.0f, 48.0f, 2.0f + 79.0f, 48.0f + 18.0f, -4.0f, -15.0f);
		spark->AddFrameData(0, 83.0f, 48.0f, 83.0f + 81.0f, 48.0f + 21.0f, -4.0f, -17.0f);
		spark->Stop();
		switch (selectCount)
		{
		case 0: Marco(scale); break;
		case 1: Eri(scale); break;
		case 2: Tarma(scale); break;
		case 3: Fio(scale); break;
		default:break;
		}
	}
	~Door() 
	{ 
		SafeDelete(image);
		SafeDelete(spark);
	};
	void Update() 
	{ 
		if (!open) OpenSequence();
		if (!close&&sequence == 1) CloseSequence();
		image->Update();
		spark->Update();

		Vector2 pos;
		image->GetTransform()->Position(&pos);
		character->SetPosition(pos.x, pos.y - 20.0f);
		capsule->SetPosition(pos.x, pos.y - 32.0f);
		character->Update();
		capsule->Update();
	};
	void Render() 
	{ 
		spark->Render();
		if (close)character->Render();
		if (open)capsule->Render();		
		image->Render();
	};
	void SetPosition(Vector2 Position) { image->SetPosition(Position); };
	void SetPosition(float x, float y) { image->SetPosition(x,y); };
	void SetBound(float x, float top, float bottom) { this->x = x; this->top = top; this->bottom = bottom; }
	void OpenSequence()
	{
		if (delayTime == 0.0f) delayTime = Time::Get()->Running();
		float curTime = image->GetPerFrame()->GetTime();
		if (curTime - delayTime < delay+0.5f) return;
		Vector2 pos;
		image->GetTransform()->Position(&pos);
		pos.y += Time::Delta()*speed;
		if (pos.y >= top)
		{
			open = 1;
			pos.y = top;
			delayTime = 0.0f;
		}
		SetPosition(pos);
	}
	void CloseSequence()
	{
		if (delayTime == 0.0f) delayTime = Time::Get()->Running();
		float curTime = image->GetPerFrame()->GetTime();
		if (curTime - delayTime < delay) return;
		Vector2 pos;
		image->GetTransform()->Position(&pos);
		pos.y -= Time::Delta()*speed;
		if (pos.y <= bottom)
		{
			close = 1;
			pos.y = bottom;
			delayTime = 0.0f;
			spark->PlayOnceAndDisappear();
			spark->SetPosition(pos.x, pos.y-188.0f);
		}
		SetPosition(pos);
	}
	void NextSequence() { sequence++; }
	void Marco(Vector2 scale)
	{
		character = new Animation(directory + L"Door_Marco.png");
		character->SetScale(scale);
		character->SetPerSecond(10.0f);	
		character->ReadFrameDatas(0);
		/*character->AddFrameData(0,0.0f, 0.0f, 42.0f, 46.0f, 0.0f, 0.0f);
		character->AddFrameData(0,41.0f, 0.0f, 83.0f, 46.0f, 0.0f, 0.0f);
		character->AddFrameData(0,82.0f, 0.0f, 124.0f, 46.0f, 0.0f, 0.0f);
		character->AddFrameData(0,123.0f, 0.0f, 165.0f, 46.0f, 0.0f, 0.0f);
		character->AddFrameData(0,82.0f, 0.0f, 124.0f, 46.0f, 0.0f, 0.0f);
		character->AddFrameData(0,41.0f, 0.0f, 83.0f, 46.0f, 0.0f, 0.0f);
		character->AddFrameData(0,0.0f, 0.0f, 42.0f, 46.0f, 0.0f, 0.0f);*/
	}
	void Eri(Vector2 scale)
	{
		character = new Animation(directory + L"Door_Eri.png");
		character->SetScale(scale);
		character->SetPerSecond(10.0f);
		character->ReadFrameDatas(0);
		/*character->AddFrameData(0,0.0f, 0.0f, 32.0f, 44.0f, 0.0f, 0.0f);
		character->AddFrameData(0,34.0f, 0.0f, 66.0f, 44.0f, 0.0f, 0.0f);
		character->AddFrameData(0,68.0f, 0.0f, 100.0f, 44.0f, -4.0f, 0.0f);
		character->AddFrameData(0,102.0f, 0.0f, 134.0f, 44.0f, -4.0f, 0.0f);
		character->AddFrameData(0,133.0f, 0.0f, 165.0f, 44.0f, -2.0f, 0.0f);
		character->AddFrameData(0,165.0f, 0.0f, 201.0f, 44.0f, 4.0f, 0.0f);
		character->AddFrameData(0,202.0f, 0.0f, 240.0f, 44.0f, 4.0f, 0.0f);
		character->AddFrameData(0,238.0f, 0.0f, 275.0f, 44.0f, -1.0f, 0.0f);
		character->AddFrameData(0,272.0f, 0.0f, 307.0f, 44.0f, -3.0f, 0.0f);
		character->AddFrameData(0,307.0f, 0.0f, 339.0f, 44.0f, 0.0f, 0.0f);
		character->AddFrameData(0,337.0f, 0.0f, 367.0f, 44.0f, 0.0f, 0.0f);
		character->AddFrameData(0,366.0f, 0.0f, 396.0f, 44.0f, 0.0f, 0.0f);
		character->AddFrameData(0,396.0f, 0.0f, 426.0f, 44.0f, 0.0f, 0.0f);
		character->AddFrameData(0,427.0f, 0.0f, 459.0f, 44.0f, 0.0f, 0.0f);
		character->AddFrameData(0,457.0f, 0.0f, 489.0f, 44.0f, 0.0f, 0.0f);
		character->AddFrameData(0,490.0f, 0.0f, 522.0f, 44.0f, 0.0f, 0.0f);*/
	}
	void Tarma(Vector2 scale)
	{
		character = new Animation(directory + L"Door_Tarma.png");
		character->SetScale(scale);
		character->SetPerSecond(10.0f);
		character->ReadFrameDatas(0);
		//character->AddFrameData(0,1.0f, 0.0f, 33.0f, 50.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,38.0f, 0.0f, 72.0f, 50.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,75.0f, 0.0f, 115.0f, 50.0f, -6.0f, 0.0f);
		//character->AddFrameData(0,114.0f, 0.0f, 154.0f, 50.0f, -6.0f, 0.0f);
		//character->AddFrameData(0,154.0f, 0.0f, 188.0f, 50.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,188.0f, 0.0f, 222.0f, 50.0f, 2.0f, 0.0f);
		//character->AddFrameData(0,224.0f, 0.0f, 259.0f, 50.0f, 7.0f, 0.0f);
		//character->AddFrameData(0,262.0f, 0.0f, 297.0f, 50.0f, 7.0f, 0.0f);
		//character->AddFrameData(0,301.0f, 0.0f, 336.0f, 50.0f, 9.0f, 0.0f);
		//character->AddFrameData(0,337.0f, 0.0f, 372.0f, 50.0f, 5.0f, 0.0f);
		//character->AddFrameData(0,375.0f, 0.0f, 410.0f, 50.0f, 7.0f, 0.0f);
		//character->AddFrameData(0,411.0f, 0.0f, 446.0f, 50.0f, 9.0f, 0.0f);
		//character->AddFrameData(0,445.0f, 0.0f, 480.0f, 50.0f, 5.0f, 0.0f);
		//character->AddFrameData(0,480.0f, 0.0f, 515.0f, 50.0f, 5.0f, 0.0f);
		//character->AddFrameData(0,517.0f, 0.0f, 552.0f, 50.0f, 3.0f, 0.0f);
		//character->AddFrameData(0,555.0f, 0.0f, 593.0f, 50.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,598.0f, 0.0f, 639.0f, 50.0f, -7.0f, 0.0f);
		//character->AddFrameData(0,646.0f, 0.0f, 694.0f, 50.0f, -8.0f, 0.0f);
		//character->AddFrameData(0,700.0f, 0.0f, 748.0f, 50.0f, -6.0f, 0.0f);
		//character->AddFrameData(0,754.0f, 0.0f, 802.0f, 50.0f, -6.0f, 0.0f);
		//character->AddFrameData(0,803.0f, 0.0f, 851.0f, 50.0f, -8.0f, 0.0f);
		//character->AddFrameData(0,850.0f, 0.0f, 898.0f, 50.0f, -10.0f, 0.0f);
	}
	void Fio(Vector2 scale)
	{
		character = new Animation(directory + L"Door_Fio.png");
		character->SetScale(scale);
		character->SetPerSecond(10.0f);
		character->ReadFrameDatas(0);
		//character->AddFrameData(0,0.0f, 0.0f, 28.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,28.0f, 0.0f, 56.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,56.0f, 0.0f, 84.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,84.0f, 0.0f, 112.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,112.0f, 0.0f, 140.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,140.0f, 0.0f, 168.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,168.0f, 0.0f, 196.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,196.0f, 0.0f, 224.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,225.0f, 0.0f, 253.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,253.0f, 0.0f, 281.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,281.0f, 0.0f, 309.0f, 31.0f, 0.0f, 0.0f);
		//character->AddFrameData(0,309.0f, 0.0f, 337.0f, 31.0f, 0.0f, 0.0f);
	}
	bool Open() { return open; };
	bool Close() { return close; };
private:
	Sprite* image;
	Sprite* capsule;
	Animation* spark;
	Animation* character;
	wstring directory;
	float x,top,bottom;//경계
	float speed;//문열리는속도
	float delay, delayTime=0.0f;//시퀀스관련
	UINT sequence=0;
	bool open = 0;
	bool close = 0;
};
