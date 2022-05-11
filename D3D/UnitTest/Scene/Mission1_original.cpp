#include "stdafx.h"
#include "Mission1.h"
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
	///카메라 시작점 설정
	Context::Get()->GetCamera()->Position(width*0.5f - x2, height*0.5 - y2 + 32.0f * ground->GetScale().y, -1.0f);
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
	///난파된 보트
	boat = new Boat(x2, y2, directory, scale, colliderManager);
	///난파선
	ship=new Ship(x2, y2, directory, scale);

	//test = new Eri(projectileManager, colliderManager,Vector2(-2782.0f, 200.0f));
	player = new Eri(projectileManager, colliderManager,Vector2(width*0.5f - x2, 150.0f),Vector2(1.3f, 1.3f));
	//test2 = new Crab(1,projectileManager, colliderManager,Vector2(width*0.5f+300.0f - x2, 150.0f), Vector2(1.3f, 1.3f));

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
}

void Mission1::Update()
{

	projectileManager->Update();

	CalcBackgroundGap();

	player->Update();
	//if(test2)test2->Update();
	background->Update();
	ground->Update();
	wave->Update();
	for (int i = 0; i < 5; i++)	fish[i]->Update();	
	rocket->Update();
	boat->Update();
	ship->Update();

	ground_Line->Update();
	ground_LineEdit->Update();

	if (!followCam)return;
	Vector3 pos;
	Context::Get()->GetCamera()->Position(&pos);
	Context::Get()->GetCamera()->Position(player->GetPosition().x,pos.y,pos.z);
}

void Mission1::Render()
{
	//Debug
	ground_Line->Render();
	
	projectileManager->Render();

	fish[2]->Render();
	wave->RenderCover();
	rocket->RenderCover();
	boat->RenderCover();
	ship->RenderCover();
	//캐릭터렌더링자리
	player->Render();
	//test2->Render();
	for (int i = 0; i < 5; i++) 
		if(i!=2)fish[i]->Render();
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
		projectileManager->AddBullet(0, new Bullet_Pistol(1, 0, 25.0f, projectileManager,colliderManager));
	projectileManager->Add(30);//헤비머신건
	for (int i = 0; i < 30; i++)
		projectileManager->AddBullet(1,new Bubble(1, 0, 20.0f, projectileManager, colliderManager));
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
////////////////Wave////////////////////////////////////////////////////////////////////////////////////////////////////////////
Wave::Wave(float x2, float y2, wstring directory, Vector2 scale, ColliderManager* colliderManager)
	:colliderManager(colliderManager)
{
	wave_Lower = new Animation(directory + L"Mission_1_Tide_Lower.png");
	wave_Lower->SetScale(scale); wave_Lower->ReadFrameDatas(0);
	wave_Lower->SetPosition(
		wave_Lower->GetTextureSize(0).x*scale.x - x2,
		40.0f*scale.y - y2 + 32.0f * scale.y);
	wave_Upper = new Animation(directory + L"Mission_1_Tide_Upper.png");
	wave_Upper->SetScale(scale); wave_Upper->ReadFrameDatas(0);
	wave_Upper->SetPosition(
		wave_Upper->GetTextureSize(0).x*scale.x - x2,
		104.0f*scale.y - y2 + 32.0f * scale.y);
	splash = new Animation(L"character/effect/Splash_Water.png");
	splash->SetScale(scale*0.2f); splash->ReadFrameDatas(0);
	splash->SetPerSecond(20.0f);splash->SetSheetOffset(0,0.0f,25.0f);

	left = -x2; right = left + (wave_Lower->GetTextureSize(0).x - 10.0f)*scale.x*2.0f;
	top = 80.0f*scale.y - y2 + 32.0f * scale.y;
}

Wave::~Wave()
{
	SafeDelete(splash);
	SafeDelete(wave_Upper);
	SafeDelete(wave_Lower);
}

void Wave::Update()
{
	splash->Update();
	wave_Upper->Update();
	wave_Lower->Update();
}

void Wave::RenderCover()
{
	if (left<=target->GetPosition().x&&
		target->GetPosition().x<=right&&
		target->GetPosition().y<=top)
	{
		splash->SetPosition(target->GetPosition().x, top);
		splash->Render();
	}
	wave_Lower->Render();
}

void Wave::RenderBackground()
{
	wave_Upper->Render();
}
void Wave::SetPlayerTarget(ICharacter * target)
{
	colliderManager->FindPlayerPosition(&this->target);
}
////////////////Rocket//////////////////////////////////////////////////////////////////////////////////////////////////////////
Rocket::Rocket(float x2, float y2,wstring directory,Vector2 scale, ColliderManager* colliderManager)
	:colliderManager(colliderManager)
{
	deafault = new Sprite(directory + L"Misson_1_Rocket_deafault.png", scale);
	deafault->SetPosition((deafault->GetTextureSize().x + 1328)*scale.x - x2, (deafault->GetTextureSize().y + 32)*scale.y - y2);
	deafault_hit = new Sprite(directory + L"Misson_1_Rocket_deafault_hit.png", scale);
	deafault_hit->SetPosition((deafault_hit->GetTextureSize().x + 1620)*scale.x - x2, (deafault_hit->GetTextureSize().y + 178)*scale.y - y2);
	deafault_cover = new Sprite(directory + L"Misson_1_Rocket_deafault_cover.png", scale);
	deafault_cover->SetPosition((deafault_cover->GetTextureSize().x + 1650)*scale.x - x2, (deafault_cover->GetTextureSize().y + 178)*scale.y - y2);
	broken = new Sprite(directory + L"Misson_1_Rocket_broken_back.png", scale);
	broken->SetPosition((broken->GetTextureSize().x + 1248)*scale.x - x2, (broken->GetTextureSize().y + 32)*scale.y - y2);
	broken_cover = new Sprite(directory + L"Misson_1_Rocket_broken_cover.png", scale);
	broken_cover->SetPosition((broken_cover->GetTextureSize().x + 1296)*scale.x - x2, (broken_cover->GetTextureSize().y + 32)*scale.y - y2);
	hitColldier=new Collider(Vector2((deafault_hit->GetTextureSize().x + 1620)*scale.x - x2, (deafault_hit->GetTextureSize().y + 178)*scale.y - y2),Vector2(80.0f,100.0f),scale);
	teamNumb = 1;
	health_Current = 200.0f;
	health_Max = 200.0f;
	colliderManager->AddHitObjects((IDamage*)this);

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
}

void Rocket::Update()
{
	//static float Rocketx = 0.0f, Rockety = 0.0f;
	//ImGui::Begin("Rocket");
	//ImGui::SliderFloat("offsetX", &Rocketx, -100.0f, 100.0f);
	//ImGui::SliderFloat("offsetY", &Rockety, -100.0f, 100.0f);
	//hitColldier->SetOffset(Rocketx, Rockety);
	//ImGui::End();

	ground_Line->Update();
	ground_LineEdit->Update();
	hitColldier->Update();
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
	if (CheckDead())
	{
		Damaged = 1;
		colliderManager->DeleteHitObjetcs((IDamage*)this);
		colliderManager->DeleteGrounds(ground_Line);
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
////////////////WreckedBoat//////////////////////////////////////////////////////////////////////////////////////////////////////////
Boat::Boat(float x2, float y2, wstring directory, Vector2 scale, ColliderManager* colliderManager)
{
	deafault = new Sprite(directory + L"Misson_1_Boat_deafault.png", scale);
	deafault->SetPosition((deafault->GetTextureSize().x + 1844)*scale.x - x2, (deafault->GetTextureSize().y + 32)*scale.y - y2);
	broken = new Sprite(directory + L"Misson_1_Boat_back.png", scale);
	broken->SetPosition((broken->GetTextureSize().x + 1844)*scale.x - x2, (broken->GetTextureSize().y + 32)*scale.y - y2);
	broken_cover = new Sprite(directory + L"Misson_1_Boat_cover.png", scale);
	broken_cover->SetPosition((broken_cover->GetTextureSize().x + 1844)*scale.x - x2, (broken_cover->GetTextureSize().y + 32)*scale.y - y2);
}

Boat::~Boat()
{
	SafeDelete(deafault);
	SafeDelete(broken);
	SafeDelete(broken_cover);
}

void Boat::Update()
{
	deafault->Update();
	broken->Update();
	broken_cover->Update();
}

void Boat::RenderCover()
{
	if (Damaged)broken_cover->Render();
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

}
bool Boat::CheckDead()
{
	return false;
}
Collider * Boat::GetHitCollider()
{
	return nullptr;
}
////////////////WreckedShip//////////////////////////////////////////////////////////////////////////////////////////////////////////
Ship::Ship(float x2, float y2, wstring directory, Vector2 scale)
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

	cover_End = new Sprite(directory + L"Misson_1_Ship_cover_End.png", scale);

	x = (door->GetTextureSize(0).x + 3005)*scale.x - x2; y = (door->GetTextureSize(0).y + 149)*scale.y - y2;
	sx = scale.x; sy = scale.y;
}

Ship::~Ship()
{
	SafeDelete(back);
	SafeDelete(door);
	SafeDelete(cover);
	SafeDelete(cover_End);
}

void Ship::Update()
{
	back->Update();
	door->Update();
	cover->Update();
	cover_End->Update();
	ImGui::SliderFloat("x", &a,-100,100);
	ImGui::SliderFloat("y", &b,-50,50);
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
	door->SetPosition(x+(a*sx), y+(b*sy));
}

void Ship::RenderCover()
{
	if (EvenetEnd)
	{
		cover_End->Render();
		return;
	}
	cover->Render();
}

void Ship::RenderBackground()
{
	if (EvenetEnd)return;
	door->Render();
	back->Render();
}

void Ship::Event()
{
	EvenetEnd = 1;
	back->PlayOnce(1);
	door->PlayOnce(1);
}
