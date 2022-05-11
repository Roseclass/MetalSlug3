#include "stdafx.h"
#include "Monster_Hermit.h"
#include <iostream>

Hermit::Hermit(ProjectileManager * manager, ColliderManager * colliderManager, Vector2 Position, Vector2 Scale)
	:position(Position),scale(Scale), manager(manager), colliderManager(colliderManager)
{
	Init();
	SetPosition(position);
	SetScale(scale);
	leftPoint = -1200.0f;
	rightPoint += leftPoint;
	colliderManager->AddHitObjects((IDamage*)this);
	teamNumb = 1;
}

Hermit::~Hermit()
{
	SafeDelete(ani);
	SafeDelete(waterSplash);
	SafeDelete(muzzleFlash);
	SafeDelete(hitCollider);
	SafeDelete(meleeCollider);
	SafeDelete(movement);
}

void Hermit::Update()
{
	static float x = 0.0f;
	static float y = 0.0f;
	//static float sx = 0.0f;
	//static float sy = 0.0f;
	//ImGui::Begin("Hermit");
	//{
	//	ImGui::SliderFloat("offsetX", &x, 0.0f, 20.0f);
	//	ImGui::SliderFloat("offsetY", &y, -500.0, 500.0f);
	//	hitCollider->SetScale(x, x);
	//	ImGui::SliderFloat("sizeX", &sx, 0.0, 500.0f);
	//	ImGui::SliderFloat("sizeY", &sy, 0.0, 500.0f);
	//	legCollider->SetSize(sx, sy);
	//	SetPosition(position);
	//}ImGui::End();
	//FSM();
	if (moveLeft)Retreat();
	else if (moveRight)Rush();
	if (state != Hermit_State::Hermit_Death)
	{
		ColliderUpdate();
	}
	else
	{
		position.y -= 20.0f*Time::Delta();
		SetPosition(position);
	}
	if (position.x <= leftPoint)
	{
		position.x += 100.0f*Time::Delta();
		SetPosition(position);
	}
	ani->Update();
	waterSplash->Update();	
	muzzleFlash->Update();
}

void Hermit::Render()
{
	ani->Render();
}

void Hermit::TakeDamage(float damage)
{
	health_Current -= damage;
	ani->SetLerpTime(0.2f);
	if (health_Current <= 0.3f*health_Max && !isRage)
	{
		isRage = 1;
		AIcount = 0;
	}
	if (CheckDead())Death();
}

bool Hermit::CheckDead()
{
	if (health_Current <= 0.0f)return 1;
	return false;
}

Collider * Hermit::GetHitCollider()
{
	return hitCollider;
}

void Hermit::RenderCover()
{
	if (state != Hermit_State::Hermit_Death)
	{
		hitCollider->Render();
		if(isMelee)meleeCollider->Render();
		legCollider->Render();
	}
	waterSplash->Render();
	muzzleFlash->Render();
}

void Hermit::SetScale(float x, float y)
{
	ani->SetScale(x, y);
	waterSplash->SetScale(x, y);
	muzzleFlash->SetScale(x, y);
	hitCollider->SetScale(x, y);
	meleeCollider->SetScale(x, y);
	legCollider->SetScale(x, y);
}

void Hermit::SetScale(Vector2 pos)
{
	SetScale(pos.x, pos.y);
}

void Hermit::SetPosition(float x, float y)
{
	ani->SetPosition(x, y);
	waterSplash->SetPosition(x+waterOffset.x*scale.x, y+waterOffset.y*scale.y);
	muzzleFlash->SetPosition(x+ muzzleOffset.x*scale.x, y+ muzzleOffset.y*scale.y);
	hitCollider->SetPosition(x, y);
	meleeCollider->SetPosition(x, y);
	legCollider->SetPosition(x, y);
}

void Hermit::SetPosition(Vector2 pos)
{
	SetPosition(pos.x, pos.y);
}

void Hermit::SetLeft(float left)
{
	this->leftPoint = left;
}

void Hermit::Init()
{
	AnimInit();
	ColliderInit();
	movement = new MovementComponent((ICharacter*)this, colliderManager, 100.0f, 0.0f);
	position.y =206.0f*scale.y - Context::Get()->GetViewport()->GetHeight()*0.5f;
	teamNumb = 1;
	health_Current = 2000.0f;
	health_Max= 2000.0f;
	prevTime = Time::Get()->Running();
}

void Hermit::AnimInit()
{
	ani = new Animation();
	ani->AddSprite(L"monster/Hermit/hermit_Normal_Walk.png");
	ani->AddSprite(L"monster/Hermit/hermit_Normal_Fire.png");
	ani->AddSprite(L"monster/Hermit/hermit_Death.png");
	for (int i = 0; i < 3; i++)ani->ReadFrameDatas(i);
	for (UINT i = 0; i < Hermit_Death; i++)
	{
		ani->SetStartNotify(i, 9, [this]() {this->AnimNotify_Melee_ColliderOn(); });
		ani->SetStartNotify(i, 11, [this]() {this->AnimNotify_Melee_ColliderOff(); });
		ani->SetEndNotify(i, ani->GetFrameDatas(i).size()-1, [this]() {this->AnimNotify_AI(); });
	}
	ani->SetStartNotify(Hermit_State::Hermit_Fire, 2, [this]() {this->AnimNotify_Fire(); });
	ani->SetScale(scale);
	ani->Pass(2);
	waterSplash = new Animation();
	waterSplash->AddSprite(L"monster/Hermit/hermit_Effect_Splash.png");
	waterSplash->ReadFrameDatas(0);
	waterSplash->SetScale(scale);
	waterOffset = Vector2(0.0f, -170.0f);
	muzzleFlash = new Animation();
	muzzleFlash->AddSprite(L"monster/Hermit/hermit_Effect_Fire.png");
	muzzleFlash->ReadFrameDatas(0);
	muzzleFlash->SetScale(scale);
	muzzleOffset = Vector2(65.0f, 140.0f);
	projectileOffset = Vector2(-15.0f, 10.0f);
	muzzleFlash->Stop();
}

void Hermit::ColliderInit()
{
	{
		vector<Vector3> temp;
		temp.push_back(Vector3(-175.0f,175.0f,0.0f));
		temp.push_back(Vector3(-175.0f,-230.0f,0.0f));
		temp.push_back(Vector3(175.0f,-230.0f,0.0f));
		temp.push_back(Vector3(175.0f,0.0f,0.0f));
		temp.push_back(Vector3(0.0f,0.0f,0.0f));
		temp.push_back(Vector3(0.0f,175.0f,0.0f));
		temp.push_back(Vector3(-175.0f,175.0f,0.0f));
		hitCollider = new ConvexHull(position, temp);
		hitCollider->SetOffset(0.0f, 0.0f);
		hitCollider->SetScale(scale); 
	}
	meleeCollider = new Collider(position, Vector2(32.0f, 64.0f));
	meleeCollider->SetOffset(174.0f, -147.0f);
	meleeCollider->SetScale(scale);
	legCollider = new Collider(position, Vector2(140.0f, 35.0f));
	legCollider->SetOffset(25.0f, -160.0f);
	legCollider->SetScale(scale);
}

void Hermit::AI()
{
	if (isRage)
	{
		if (AIcount == 0|| AIcount == 5)	state = Hermit_State::Hermit_Walk;
		if (AIcount == 1)//앞으로돌진
		{
			state = Hermit_State::Hermit_Walk;
			moveRight = 1;
			if(position.x<rightPoint )return;
			moveRight = 0;
		}
		if(AIcount==2||AIcount==3)state = Hermit_State::Hermit_Walk;
		if(AIcount ==4) //뒤로 후퇴
		{
			state = Hermit_State::Hermit_Walk;
			moveLeft = 1;
			if (position.x > leftPoint)return;
			moveLeft = 0;
		}
		AIcount++;
		AIcount %= 6;
	}
	else
	{
		if (AIcount == 0)	state = Hermit_State::Hermit_Walk;
		if (AIcount == 1)	state = Hermit_State::Hermit_Walk;
		if (AIcount == 2)	state = Hermit_State::Hermit_Walk;
		if (AIcount == 3)	state = Hermit_State::Hermit_Fire;
		if (AIcount == 4)	state = Hermit_State::Hermit_Walk;
		if (AIcount == 5)	state = Hermit_State::Hermit_Walk;
		if (AIcount == 6)	state = Hermit_State::Hermit_Walk;
		AIcount++;
		AIcount %= 6;
	}	
	FSM();
}

void Hermit::FSM()
{
	if (prevState != Hermit_State::Hermit_Max)return;
	switch (state)
	{
	case Hermit_Walk:Walk(); break;
	case Hermit_Fire:Fire(); break;
	default:break;
	}
	prevState = state;
}

void Hermit::ColliderUpdate()
{
	if (isMelee)
	{
		colliderManager->IntersectAllObjects(meleeCollider, teamNumb, 100.0f);
		meleeCollider->Update();
	}
	colliderManager->IntersectAllObjects(legCollider, teamNumb, 100.0f);
	legCollider->Update();
	hitCollider->Update();
}

void Hermit::Walk()
{
	{
		state = Hermit_State::Hermit_Walk;
		ani->PlayLoop(Hermit_State::Hermit_Walk);
	}
}

void Hermit::Fire()
{
	{
		state = Hermit_State::Hermit_Fire;
		ani->PlayLoop(Hermit_State::Hermit_Fire);
	}
}

void Hermit::Death()
{
	state = Hermit_State::Hermit_Death;
	ani->PlayLoop(Hermit_State::Hermit_Death);
}

void Hermit::Rush()
{
	movement->MoveForward(position, 0);
	if (position.x > rightPoint)position.x = rightPoint;
	SetPosition(position);
	perSecond += Time::Delta()*10.0f;
	if (perSecond > defaultPerSecond*2.0f)perSecond = defaultPerSecond*2.0f;
	ani->SetPerSecond(perSecond);
}

void Hermit::Retreat()
{
	movement->MoveForward(position, 1);
	if (position.x < leftPoint)position.x = leftPoint;
	SetPosition(position);
	perSecond -= Time::Delta()*10.0f;
	if (perSecond < defaultPerSecond)perSecond = defaultPerSecond;
	ani->SetPerSecond(perSecond);
}

void Hermit::AnimNotify_AI()
{
	AI();
	prevState= Hermit_State::Hermit_Max;
}

void Hermit::AnimNotify_Melee_ColliderOn()
{
	isMelee = 1;
}

void Hermit::AnimNotify_Melee_ColliderOff()
{
	isMelee = 0;
}

void Hermit::AnimNotify_Fire()
{
	muzzleFlash->PlayOnceAndDisappear();
	Vector2 temp;
	temp.x = position.x + (muzzleOffset.x + projectileOffset.x)*scale.x;
	temp.y = position.y + (muzzleOffset.y + projectileOffset.y)*scale.y;
	muzzleFlash->SetPosition(temp.x, temp.y);
	manager->Activate(2, temp, 0.0f);
}

Hermit_FireBall::Hermit_FireBall(UINT teamNumber, UINT poolIndex, float damage, ProjectileManager * manager, ColliderManager * colliderManager)
	:Bullet(teamNumber,poolIndex,damage, manager, colliderManager)
{
	collider = new Collider(Vector2(0.0f, 0.0f), Vector2(10.0f, 10.0f));
	ani = new Animation();
	ani->AddSprite(L"monster/Hermit/hermit_Projectile_Fireball.png");
	ani->SetPerSecond(15.0f);
	ani->ReadFrameDatas(0);
	for (int i = 0; i < 20; i++)
	{
		trail[i] = new Animation();
		trail[i]->AddSprite(L"monster/Hermit/hermit_Effect_FireBall_Trail.png");
		trail[i]->SetPerSecond(10.0f);
		trail[i]->ReadFrameDatas(0);
		trail[i]->SetScale(scale.x*1.5f, scale.y*1.5f);
		trail[i]->Stop();
	}
	lifespan = 5.0f;
	activationTime = Time::Get()->Running();
	this->teamNumber = teamNumber;
}

Hermit_FireBall::~Hermit_FireBall()
{
	for(int i =0; i<10; i++)SafeDelete(trail[i]);
	SafeDelete(effect);
}

void Hermit_FireBall::Update()
{
	for (int i = 0; i < 20; i++)trail[i]->Update();
	if (!activated) return;
	if (Time::Get()->Running() - activationTime >= lifespan)
	{
		Pop();
	}
	if (ani->IsPlay() == 0)
	{
		Deactivate();
		return;
	}
	Progress();
	CollisionCheck();
	Bullet::Update();
}

void Hermit_FireBall::Render()
{
	if (activated)
	{
		Bullet::Render();
		ani->Render();
	}
	for (int i = 0; i < 20; i++)trail[i]->Render();
}

void Hermit_FireBall::Progress()
{
	Vector3 temp = ani->GetPosition();
	forward.x = cos(rotZ*Radian);
	forward.y = sin(rotZ*Radian);
	temp += forward * 200.0f*Time::Delta();
	ani->SetPosition(temp.x,temp.y);
	collider->SetPosition(temp.x, temp.y);
	rotZ -= 40.0f*Time::Delta();
	if (rotZ <= -70.0f)rotZ = -70.0f;
	if (pivot > 19)return;
	if (Time::Get()->Running() - activationTime >= pivot * 0.25f)
	{
		trail[pivot]->PlayOnceAndDisappear(0);
		trail[pivot]->SetRotationDegree(0.0f, 0.0f, rotZ);
		trail[pivot]->SetPosition(temp.x,temp.y);
		pivot++;
	}
}

void Hermit_FireBall::Activate(float z)
{
	activationTime = Time::Get()->Running();
	activated = 1;
	rotZ = 45.0f;
}

void Hermit_FireBall::Deactivate()
{
	activationTime = 0.0f;
	activated = 0;
	pivot = 0;
}

void Hermit_FireBall::CollisionCheck()
{
	colliderManager->IntersectObjects((Bullet*)this);
	colliderManager->IntersectGrounds((Bullet*)this);
}

void Hermit_FireBall::Pop()
{
	Deactivate();
}

void Hermit_FireBall::SetScale(float x, float y)
{
	Bullet::SetScale(x,y);
	for (int i = 0; i < 20; i++)trail[i]->SetScale(scale.x*1.5f, scale.y*1.5f);
}

void Hermit_FireBall::SetScale(Vector2 pos)
{
	Bullet::SetScale(pos);
	for (int i = 0; i < 20; i++)trail[i]->SetScale(scale.x*1.5f, scale.y*1.5f);
}
