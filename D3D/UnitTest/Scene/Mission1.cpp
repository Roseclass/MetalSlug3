#include "stdafx.h"
#include "Main.h"
#include "Mission1.h"
#include "Mission1_Boat.h"
#include "Mission1_Rocket.h"
#include "Mission1_Wave.h"
#include "Mission1_Ship.h"
#include "Characters/Monster_Crab.h"
#include "Characters/Player_Eri.h"
#include "Characters/Player_Bullet.h"
#include "LineEdit/LineEdit.h"
#include "Objects/Projectile.h"
#include "Objects/ColliderManager.h"

void Mission1::Initialize()
{
	ground_Line = new Line();
	ground_LineEdit = new LineEdit(ground_Line,"Mission1_Ground");

	float height= Context::Get()->GetViewport()->GetHeight();
	float width= Context::Get()->GetViewport()->GetWidth();
	directory = L"background/Mission_1/";
	scale = Vector2(width / 306.0f*0.5f, height / 225.0f*0.5f);

	CreateColliderManager();
	CreateProjectileManager();

	///뒷배경
	background = new Sprite(directory + L"Misson_1_Back.png",Vector2(1.6f, height/208.0f * 0.5f));
	float x1, y1;
	x1 = background->GetTextureSize().x*background->GetScale().x;
	y1 = background->GetTextureSize().y*background->GetScale().y;
	///땅
	ground = new Sprite(directory + L"Misson_1_Ground.png", scale);
	float x2, y2;
	x2 = ground->GetTextureSize().x*scale.x;
	y2 = ground->GetTextureSize().y*scale.y;
	background->SetPosition(Vector2(x1 - x2, y1 - y2 + 32.0f * ground->GetScale().y));
	///배경,땅 거리계산 재료
	gap = x2 * 2 - x1 * 2;
	backstart = x1 - x2;
	Groundstart = width - x2;
	x = x2 * 2 - width;

	//Context::Get()->GetCamera()->Position(-2682.0f, height*0.5 - y2 + 32.0f * ground->GetScale().y, -1.0f);
	///파도
	wave= new Wave(x2, y2, directory, scale,colliderManager);
	///생선
	fish[0] = new Sprite(directory+L"Misson_1_Fish00.png", scale);
	fish[0]->SetPosition((fish[0]->GetTextureSize().x + 720.0f)*scale.x - x2, (fish[0]->GetTextureSize().y + 154.0f)*scale.y - y2);
	fish[1] = new Sprite(directory+L"Misson_1_Fish01_back.png", scale);
	fish[1]->SetPosition((fish[1]->GetTextureSize().x + 918.0f)*scale.x - x2, (fish[1]->GetTextureSize().y + 96.0f)*scale.y - y2);
	fish[2] = new Sprite(directory+L"Misson_1_Fish01_cover.png", scale);
	fish[2]->SetPosition((fish[2]->GetTextureSize().x + 976.0f)*scale.x - x2, (fish[2]->GetTextureSize().y + 96.0f)*scale.y - y2);
	fish[3] = new Sprite(directory+L"Misson_1_Fish02.png", scale);
	fish[3]->SetPosition((fish[3]->GetTextureSize().x + 1144.0f)*scale.x - x2, (fish[3]->GetTextureSize().y + 70.0f)*scale.y - y2);
	fish[4] = new Sprite(directory+L"Misson_1_Fish03.png", scale);
	fish[4]->SetPosition((fish[4]->GetTextureSize().x + 1304.0f)*scale.x - x2, (fish[4]->GetTextureSize().y + 58.0f)*scale.y - y2);
	///로켓
	rocket = new Rocket(x2, y2, directory, scale, colliderManager);
	rocket->AddEvent([this]() {this->Event_Phase_Rocket(); });
	///난파된 보트
	boat = new Boat(x2, y2, directory, scale, colliderManager);
	boat->AddEvent([this]() {this->Event_Phase_Boat(); });
	///난파선
	ship=new Ship(x2, y2, directory, scale, colliderManager);

	{
		player = new Eri(projectileManager, colliderManager, Vector2(150.0f - x2, 150.0f), Vector2(1.3f, 1.3f)); 
		followCam = 1;
	}
	monster_Crab[0] = new Crab(1,projectileManager, colliderManager,Vector2(0.0f,0.0f), Vector2(1.3f, 1.3f));
	monster_Crab[1] = new Crab(1,projectileManager, colliderManager,Vector2(0.0f,0.0f), Vector2(1.3f, 1.3f));
	monster_Crab[2] = new Crab(1,projectileManager, colliderManager,Vector2(0.0f,0.0f), Vector2(1.3f, 1.3f));
	monster_Crab[3] = new Crab(0,projectileManager, colliderManager,Vector2(0.0f,0.0f), Vector2(1.3f, 1.3f));
	monster_Crab[4] = new Crab(0,projectileManager, colliderManager,Vector2(0.0f,0.0f), Vector2(1.3f, 1.3f));
	wave->SetPlayerTarget((ICharacter*)player);
}

void Mission1::Destroy()
{
	SafeDelete(background);
	SafeDelete(ground);
	SafeDelete(wave);
	for (int i = 0; i < 5; i++)	SafeDelete(fish[i]);
	SafeDelete(rocket);
	SafeDelete(boat);
	SafeDelete(ship);

	SafeDelete(ground_Line);
	SafeDelete(ground_LineEdit);

	for (int i = 0; i < 5; i++)	SafeDelete(monster_Crab[i]);
	SafeDelete(player);
	SafeDelete(ground_LineEdit);
	SafeDelete(projectileManager);
}

void Mission1::Update()
{
	static float posx=-6000.0f, posy = 0.0f;
	//static float sizex=0.0f, sizey = 0.0f;
	ImGui::SliderFloat("posx", &posx, -4000.0f, 0.0f);
	ImGui::SliderFloat("posy", &posy,-200.0f,500.0f );
	//monster_Crab[4]->SetPosition(posx, posy);
	//trigger->SetSize(sizex, sizey);
	projectileManager->Update();

	CalcBackgroundGap();
	
	player->Update();
	background->Update();
	ground->Update();
	wave->Update();
	for (int i = 0; i < 5; i++)
	{
		fish[i]->Update();
		monster_Crab[i]->Update();
	}
	rocket->Update();
	boat->Update();
	ship->Update();

	ground_Line->Update();
	ground_LineEdit->Update();

	trigger->Update();

	if (!followCam)return;
	Vector3 pos; float x=player->GetPosition().x;
	Context::Get()->GetCamera()->Position(&pos);
	if (abs(pos.x - x) > 3.0f)
		x = (pos.x*0.99f) + (x*0.01f);
	cam->SetPosition(x,pos.y,pos.z);
}

void Mission1::Render()
{
	//Debug
	ground_Line->Render();
	trigger->Render();

	projectileManager->Render();

	fish[2]->Render();
	wave->RenderCover();
	rocket->RenderCover();
	boat->RenderCover();
	ship->RenderCover();
	//캐릭터렌더링자리
	player->Render();
	for (int i = 0; i < 5; i++)	monster_Crab[i]->Render();
	for (int i = 0; i < 5; i++)	if(i!=2)fish[i]->Render();
	rocket->RenderBackground();
	wave->RenderBackground();
	boat->RenderBackground();
	ship->RenderBackground();
	ground->Render();
	background->Render();
}

void Mission1::CreateProjectileManager()
{
	projectileManager = new ProjectileManager();
	projectileManager->Add(30);//피스톨
	for (int i = 0; i < 30; i++)
		projectileManager->AddBullet(0, new Bullet_Pistol(0, 0, 25.0f, projectileManager,colliderManager));
	projectileManager->Add(30);//헤비머신건
	for (int i = 0; i < 30; i++)
		projectileManager->AddBullet(1,new Bullet_HeavyMG(1, 0, 25.0f, projectileManager, colliderManager));
	projectileManager->Add(30);//크랩
	for (int i = 0; i < 30; i++)
		projectileManager->AddBullet(2,new Bubble(1, 0, 20.0f, projectileManager, colliderManager));
}

void Mission1::CreateColliderManager()
{
	colliderManager = new ColliderManager();
	colliderManager->AddGrounds(ground_Line);
	ground_LineEdit->LoadFile();
}

void Mission1::CalcBackgroundGap()
{
	Vector3 X,Y;
	background->GetTransform()->Position(&Y);
	Context::Get()->GetCamera()->Position(&X);
	X.x = (X.x - Groundstart) / x * gap + backstart;
	background->SetPosition(X.x, Y.y);
}

void Mission1::Event_Start()
{
	float x2;
	x2 = ground->GetTextureSize().x*scale.x;
	///카메라 시작점 설정
	Context::Get()->GetCamera()->Position(Context::Get()->GetViewport()->GetHeight()*0.5f - x2, 
		Context::Get()->GetViewport()->GetHeight()*0.5 - ground->GetTextureSize().y*ground->GetScale().y + 32.0f * ground->GetScale().y, -1.0f);
	cam = (Freedom*)Context::Get()->GetCamera();
	cam->SetLeft(Context::Get()->GetViewport()->GetWidth()*0.5f - x2);
	cam->SetRight(-4205.0f);
	monster_Crab[0]->SetPosition(-5580.0f,-100.0f);
	monster_Crab[1]->SetPosition(-5850.0f,-100.0f);
	monster_Crab[2]->SetPosition(-5043.0f,0.0f);
	monster_Crab[3]->SetPosition(-4895.0f,0.0f);
	monster_Crab[4]->SetPosition(-4745.0f,0.0f);
	trigger = new Collider(Vector2(-6700.0f,0.0f),Vector2(10.0f,500.0f));
	colliderManager->AddTriggers(trigger, [this]() {
		this->Event_Fish();
		this->monster_Crab[0]->StartEat();
		this->monster_Crab[0]->SetNoticeRange(Vector2(200.0f,200.0f));
		this->trigger->SetPosition(-4915.0f, 0.0f);
	});
	main->LoadMission1_Boss();
}

void Mission1::Event_Fish()
{
	colliderManager->AddTriggers(trigger, [this]() {
		this->cam->SetStop();
		this->trigger->SetPosition(-2930.0f, 0.0f);
	});
}

void Mission1::Event_Phase_Rocket()//로켓이 터지면 실행
{
	cam->SetMove();
	cam->SetRight(-2640.0f);
	monster_Crab[0]->Reset(Vector2(-2380.0f, 230.0f));
	monster_Crab[1]->Reset(Vector2(-2800.0f, -100.0f));
	monster_Crab[2]->Reset(Vector2(-2200.0f, 230.0f));
	monster_Crab[3]->Reset(Vector2(-2200.0f, 230.0f));

	colliderManager->AddTriggers(trigger, [this]() {this->Event_Phase_Ship(); });
}

void Mission1::Event_Phase_Boat()//보트가 터지면 실행
{
	//아이템생성
}

void Mission1::Event_Phase_Ship()//일정거리내로오면 문열리고 꽃게가 뛰쳐나올거
{
	trigger->SetPosition(-2340.0f, 0.0f);
	colliderManager->AddTriggers(trigger, [this]() {this->Event_Phase_NextLevel();});
	for (int i = 0; i < 4; i++)
		monster_Crab[i]->SetTarget((ICharacter*)player);
	ship->Event();
}

void Mission1::Event_Phase_NextLevel()
{
	colliderManager->DeleteTriggers(trigger);
	LoadNextScene();
}

void Mission1::LoadNextScene()
{
	followCam = 0;
	main->NextScene();
}
