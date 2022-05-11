#include "stdafx.h"
#include "Mission1_Boss.h"
#include "Mission1_Boss_Ground.h"
#include "Objects/Projectile.h"
#include "Objects/ColliderManager.h"
#include "Objects/Items_HeavyMG.h"
#include "Characters/Monster_Hermit.h"
#include "Characters/Player_Eri.h"
#include "Characters/Player_Bullet.h"

void Mission1_Boss::Initialize()
{
	scale.x = Context::Get()->GetViewport()->GetWidth() / 512.0f * 0.5f;
	scale.y = Context::Get()->GetViewport()->GetHeight() / 192.0f * 0.5f;
	float height = (Context::Get()->GetViewport()->GetHeight()*0.5);
	background = new Sprite(L"background/Mission_1/BossStage/Mission_1_Boss_Back.png", scale);
	StartUv = Vector2(0.0f, 0.0f);EndUv = Vector2(512.0f, 192.0f);
	cam = (Freedom*)Context::Get()->GetCamera();
	background->SetPosition(0.0f, 0.0f);
	background->Pass(3);//Wrap모드

	CreateRiver();
	CreateColliderManager();
	CreateGround();
	CreateProjectileManager();
	{
		player = new Eri(projectileManager, colliderManager, Vector2(200.0f, 0.0f), Vector2(1.3f, 1.3f));
	}
	hermit = new Hermit(projectileManager, colliderManager, Vector2(-1200.0f, 0.0f)); 
	trigger = new Collider(Vector2(400.0f,-230.0f),Vector2(22.0f,20.0f),Vector2(1.3f, 1.3f));
	image = new Sprite(L"misc/Items_HeavyMG.png", Vector2(1.3f, 1.3f));
	image->SetPosition(Vector2(400.0f, -230.0f));
	colliderManager->AddTriggers(trigger, [this]() {
		this->GameStart();
		this->player->ChangeWeapon(1);
		colliderManager->DeleteTriggers(trigger);
	});
}

void Mission1_Boss::Destroy()
{
	SafeDelete(background);
	for (int i = 0; i < river_back.size(); i++)SafeDelete(river_back[i]);
	for (int i = 0; i < river_front.size(); i++)SafeDelete(river_front[i]);
	SafeDelete(hermit);
	SafeDelete(projectileManager);
	SafeDelete(colliderManager);
	for (int i = 0; i < pool.size(); i++)SafeDelete(pool[i]);
	SafeDelete(player);
	SafeDelete(trigger);
}

void Mission1_Boss::Update()
{
	//Vector3 pos;
	//cam->Position(&pos);
	//pos.x += 200.0f*Time::Delta();
	//cam->Position(pos);
	//background->SetPosition(pos.x,pos.y);
	/*if (ImGui::Button("GetBack"))
		pool.front()->GetBack();
	if (ImGui::Button("activate"))
		projectileManager->Activate(1,Vector2(0.0f,0.0f),0.0f);*/
	if(groundMove&&hermit->GetState()!=Hermit_Death)
	{
		float add = 10.0f*Time::Delta();
		StartUv.x += add; EndUv.x += add;
		player->SetPosition(player->GetPosition().x - Time::Delta()*100.0f, player->GetPosition().y);
		background->SetUv(StartUv, EndUv); for (auto i : pool)i->Move();
	}

	for (int i = 0; i < river_back.size(); i++)river_back[i]->Update();
	for (int i = 0; i < river_front.size(); i++)river_front[i]->Update();
	for (auto i : pool)i->Update();
	projectileManager->Update();
	background->Update();
	hermit->Update();
	player->Update();
	trigger->Update();
	image->Update();
}

void Mission1_Boss::Render()
{
	if (bImage)
	{
		image->Render();
		trigger->Render();
	}
	projectileManager->Render();
	hermit->RenderCover();
	player->Render();
	hermit->Render();
	for (auto i : pool)i->Render();
	for (int i = 0; i < river_front.size(); i++)river_front[i]->Render();
	for (int i = 0; i < river_back.size(); i++)river_back[i]->Render();
	background->Render();
}

void Mission1_Boss::Start()
{
	cam->Position(0.0f, 0.0f, -1.0f);
}

void Mission1_Boss::CreateProjectileManager()
{
	projectileManager = new ProjectileManager();
	projectileManager->Add(30);//피스톨
	for (int i = 0; i < 30; i++)
		projectileManager->AddBullet(0, new Bullet_Pistol(0, 0, 25.0f, projectileManager,colliderManager));
	projectileManager->Add(60);//헤비머신건
	for (int i = 0; i < 60; i++)
		projectileManager->AddBullet(1,new Bullet_HeavyMG(0, 0, 20.0f, projectileManager, colliderManager));
	projectileManager->Add(2);//크랩
	for (int i = 0; i < 2; i++)
		projectileManager->AddBullet(2,new Hermit_FireBall(1, 0, 20.0f, projectileManager, colliderManager));
}

void Mission1_Boss::CreateColliderManager()
{
	colliderManager = new ColliderManager();
}

void Mission1_Boss::CreateRiver()
{
	float width = (Context::Get()->GetViewport()->GetWidth()*0.5);
	float height = (Context::Get()->GetViewport()->GetHeight()*0.5);
	float x = width,y=height;

	Vector2 d;
	river_front.push_back(new Animation());
	river_front.back()->AddSprite(L"background/Mission_1/BossStage/Mission_1_Boss_River_Front.png");
	river_front.back()->ReadFrameDatas(0); d = river_front.back()->GetSheet(0)->GetScale();
	river_front.back()->SetPosition(Vector2(-width, 16.0f*d.y - height));
	river_front.back()->SetPerSecond(20.0f);
	while (river_front.back()->GetPosition().x < width - 128.0f*d.x)
	{
		Animation* temp = new Animation();
		temp->AddSprite(L"background/Mission_1/BossStage/Mission_1_Boss_River_Front.png");
		temp->ReadFrameDatas(0);
		temp->SetPosition(Vector2(256.0f*d.x + river_front.back()->GetPosition().x, river_front.back()->GetPosition().y));
		temp->SetPerSecond(20.0f);
		river_front.push_back(temp);
	}

	Vector2 s;
	river_back.push_back(new Animation());
	river_back.back()->AddSprite(L"background/Mission_1/BossStage/Mission_1_Boss_River_Back.png");
	river_back.back()->ReadFrameDatas(0); s = river_back.back()->GetSheet(0)->GetScale();
	river_back.back()->SetPosition(Vector2(-width, 32.0f*s.y+ 32.0f*d.y - height));
	river_back.back()->SetPerSecond(20.0f);
	while (river_back.back()->GetPosition().x < width - 144.0f*s.x)
	{
		Animation* temp = new Animation();
		temp->AddSprite(L"background/Mission_1/BossStage/Mission_1_Boss_River_Back.png");
		temp->ReadFrameDatas(0);
		temp->SetPosition(Vector2(288.0f*s.x+ river_back.back()->GetPosition().x, river_back.back()->GetPosition().y));
		temp->SetPerSecond(20.0f);
		river_back.push_back(temp);
	}

}

void Mission1_Boss::CreateGround()
{
	int i = 0;
	float height = (Context::Get()->GetViewport()->GetHeight()*0.5);
	float posx = 0.0f;
	float posy = (71.0f*1.3f) - height;
	float addx =128.0f;

	new Boss_Ground(9, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(9, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(1, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(2, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(10, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(10, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(3, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(4, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(11, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(11, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(5, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(6, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(10, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(10, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(7, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);
	new Boss_Ground(8, Vector2(1.3f, 1.3f), Vector2(i++*addx, posy), colliderManager, &pool);	
}

void Mission1_Boss::GameStart()
{
	groundMove = 1;
	bImage = 0;
	hermit->SetLeft(-285.0f);
}
