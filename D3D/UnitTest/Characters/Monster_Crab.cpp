#include "stdafx.h"
#include "Monster_Crab.h"
Crab::Crab(bool Melee, ProjectileManager* manager, ColliderManager* colliderManager, Vector2 Position, Vector2 Scale)
	:manager(manager)
	, colliderManager(colliderManager)
	, position(Position)
	, scale(Scale)
{
	teamNumb = 1;
	isMelee = Melee;
	
	//Load
	Init();
	SetPosition(Position);
	SetScale(Scale);
	ani->Pass(2);
	ani->Play();
	//Set Health
	health_Max = 100.0f;
	health_Current = 100.0f;
	teamNumb = 1;
	isLeft = 1;
	prevTime = Time::Get()->Running();
	target = nullptr;
	movement = new MovementComponent(this, colliderManager, 150.0f, 350.0f);
	if (isMelee)movement->SetSpeed(250.0f);
	colliderManager->AddHitObjects((IDamage*)this);
}

Crab::~Crab()
{
	SafeDelete(ani);
	SafeDelete(hitCollider);
	SafeDelete(meleeCollider);
	SafeDelete(noticeCollider);
}

void Crab::Update()
{
	FSM();
	if (state != Crab_State::Crab_Death)
	{
		PosUpdate();
		ColliderUpdate();
	}
	ani->Update();

	////ImGui
	//ImGui::Begin("Crab");
	//if (ImGui::Button("Notice")) notice = !notice;
	//if (ImGui::Button("Death")) health_Current = -10.0f;
	//if (ImGui::Button("Reset")) Reset(Vector2(position.x, position.y + 100.0f));
	//ImGui::End();
	//ImGui::SliderFloat("addx", &x, -500.0f, 500.0f);
	//ImGui::SliderFloat("addy", &y, -500.0f, 500.0f);
}

void Crab::Render()
{
	if (isAct)meleeCollider->Render();	
	if (state != Crab_State::Crab_Death)
	{
		if (!notice)noticeCollider->Render();
		hitCollider->Render();
	}
	ani->Render();
}

bool Crab::IsGround()
{
	Vector2 pos = position;
	if (colliderManager->IntersectGrounds(&pos))
		return 1;
	return false;
}

void Crab::TakeDamage(float damage)
{
	if (state == Crab_State::Crab_Death)return;
	ani->SetLerpTime(0.2f);
	health_Current -= damage;
	if (CheckDead())
	Death();	
}

bool Crab::CheckDead()
{
	if (health_Current <= 0.0f) return 1;
	return 0;
}

Collider* Crab::GetHitCollider()
{
	return hitCollider;
}

void Crab::Reset(Vector2 Position)
{
	state = Crab_State::Crab_Idle;
	ani->PlayLoop(state);
	prevTime = Time::Get()->Running();
	notice = 0;
	AIcount = 0;
	health_Max = 100.0f;
	health_Current = 100.0f;
	teamNumb = 1;
	target = nullptr;
	canMove = 1;
	SetPosition(Position);
	SetNoticeRange(Vector2(350.0f, 350.0f));
	colliderManager->AddHitObjects((IDamage*)this);
	if(!isLeft)Left();
}

void Crab::StartEat()
{
	if (state != Crab_State::Crab_Idle)return;
	ani->PlayLoop(Crab_Death+1);
}

void Crab::SetScale(float x, float y)
{
	if (!canMove) return;
	//position = Vector2(x, y);
	scale = Vector2(x, y);
	ani->SetScale(x, y);
	hitCollider->SetScale(x, y);
	meleeCollider->SetScale(x, y);
}

void Crab::SetScale(Vector2 pos)
{
	SetScale(pos.x, pos.y);
}

void Crab::SetRotation(float x, float y)
{
	if (!canMove) return;
	//position = Vector2(x, y);
	ani->SetRotationDegree(x, y,0.0f);
	hitCollider->SetRotationDegree(x, y, 0.0f);
}

void Crab::SetRotation(Vector2 pos)
{
	SetRotation(pos.x, pos.y);
}

void Crab::SetPosition(float x, float y)
{
	position = Vector2(x,y);
	ani->SetPosition(x, y + size*scale.y);
	hitCollider->SetPosition(x, y + size * scale.y);
	noticeCollider->SetPosition(x, y);
}

void Crab::SetPosition(Vector2 pos)
{
	SetPosition(pos.x, pos.y);
}

void Crab::SetTarget(ICharacter * player)
{
	target = player;
}

void Crab::SetNoticeRange(Vector2 range)
{
	noticeCollider->SetSize(range);
}

void Crab::Init()
{
	directory = L"/monster/Crab/";
	extent = L".png";
	ani = new Animation();
	ani->AddSprite(directory + L"Crab_Idle" + extent);
	ani->AddSprite(directory + L"Crab_Walking" + extent);
	ani->AddSprite(directory + L"Crab_Melee" + extent);
	ani->AddSprite(directory + L"Crab_Range" + extent);
	ani->AddSprite(directory + L"Crab_Jump" + extent);
	ani->AddSprite(directory + L"Crab_Death_Normal" + extent);
	ani->AddSprite(directory + L"Crab_Eating" + extent);
	for (int i = 0; i < 7; i++)	ani->ReadFrameDatas(i);
	ani->SetStartNotify(Crab_State::Crab_Range, 9, [this]() {this->AnimNotify_Shoot_Bubble(); });
	ani->SetStartNotify(Crab_State::Crab_Melee, 4, [this]() {this->AnimNotify_Act_Melee_Start(); });
	ani->SetEndNotify(Crab_State::Crab_Melee, 8, [this]() {this->AnimNotify_Act_Melee_End(); });
	ani->SetEndNotify(Crab_State::Crab_Melee, 12, [this]() {this->AnimNotify_Act_End(); });
	//ani->SetEndNotify(Crab_State::Crab_Death, ani->GetFrameDatas(Crab_State::Crab_Death).size()-1, 	[this]() {Death(); });
	ColliderInit();
}

void Crab::ColliderInit()
{
	Vector2 size = Vector2(30, 37);
	hitCollider = new Collider(position, size, scale);
	hitCollider->SetOffset(0, -8);
	size = Vector2(30, 35);
	meleeCollider = new Collider(position, size, scale);
	meleeCollider->SetOffset(-50, 4);
	size = Vector2(350.0f, 350.0f);
	noticeCollider = new Collider(position, size, scale);
	noticeCollider->SetOffset(0.0f, 100.0f);
}

void Crab::AI()
{
	if (AIcount == 0)if(Task_0_Notice())AIcount++;
	if (state == Crab_State::Crab_Floating) return;
	if (AIcount == 1)if(Task_1_MoveTo())AIcount++;
	if (AIcount == 2)if(Task_2_Attack())AIcount++;
	if (AIcount == 3)if(Task_3_Wait())AIcount++;
	AIcount %= 4;
}

void Crab::FSM()
{
	if (state == Crab_State::Crab_Death) return;
	prevState = state;
	if (!IsGround())state = Crab_State::Crab_Floating;
	else state = Crab_State::Crab_Idle;
	AI();
	if (prevState == state) return;
	switch (state)
	{
	case Crab_State::Crab_Idle:Idle();
		break;
	case Crab_State::Crab_Walk:Walk();
		break;
	//case Crab_State::Crab_Melee:Melee();
	//	break;
	//case Crab_State::Crab_Range:Range();
	//	break;
	case Crab_State::Crab_Floating:ani->PlayLoop(Crab_State::Crab_Floating);
		break;
	default:
		break;
	}
}

void Crab::CheckLeft()
{
	if (state == Crab_State::Crab_Death)return;
	Vector3 rot; ani->GetTransform()->RotationDegree(&rot);
	if (int(rot.x) % 360) isLeft = 0;
	else isLeft = 1;
}

void Crab::PosUpdate()
{
	movement->Update(position);
	SetPosition(position);
}

void Crab::ColliderUpdate()
{
	if (isAct)
	{
		if (colliderManager->IntersectObjects(meleeCollider, teamNumb, meleeDamage))isAct = 1;		
		meleeCollider->Update();
	}
	if (!notice)
	{
		if (colliderManager->FindPlayerPosition(noticeCollider, &target))
			notice = 1;		
		noticeCollider->Update();
	}
	hitCollider->Update();
}

void Crab::Idle()
{
	//state = Crab_State::Idle;
	ani->PlayLoop(Crab_State::Crab_Idle);
	canMove = 1;
}

void Crab::Walk()
{
	//state = Crab_State::Walk;
	ani->PlayLoop(Crab_State::Crab_Walk);
	canMove = 1;
}

void Crab::Melee()
{
	state = Crab_State::Crab_Melee;
	ani->PlayMontage(Crab_State::Crab_Melee);
	canMove = 0;
}

void Crab::Range()
{
	state = Crab_State::Crab_Range;
	ani->PlayMontage(Crab_State::Crab_Range);
	canMove = 0;
}

void Crab::Death()
{
	ani->SetLerpTime(0.0f);
	colliderManager->DeleteHitObjetcs((IDamage*)this);
	state = Crab_State::Crab_Death;
	ani->PlayOnceAndDisappear(Crab_State::Crab_Death);
	canMove = 0;
	isAct = 0;
}

void Crab::Left()
{
	isLeft = 1;
	ani->SetRotationDegree(0.0f, 0.0f, 0.0f);
	meleeCollider->ReverseOffsetX();
}

void Crab::Right()
{
	isLeft = 0;
	ani->SetRotationDegree(0.0f, 180.0f, 0.0f);
	meleeCollider->ReverseOffsetX();
}

bool Crab::MoveTo(Vector2 pos, float range)
{
	if (Math::Distance(pos, position) < range)return 1;
	if (!canMove)return 0;
	state = Crab_State::Crab_Walk;
	if (isLeft&&pos.x > position.x)Right();
	else if (!isLeft&&pos.x < position.x)Left();
	movement->MoveForward(position,isLeft);
	return 0;
}

bool Crab::Task_0_Notice()
{
	if (notice) return 1;
	return 0;
}

bool Crab::Task_1_MoveTo()
{
	float temp; isMelee ? temp = melee : temp = range;
	if (target == nullptr)
	{
		AIcount = 0;
		notice = 0;
		return 0;
	}
	if (MoveTo(target->GetPosition(), temp)) return 1;
	return 0;
}

bool Crab::Task_2_Attack()
{
	if (isMelee&&ani->IsPlay(Crab_State::Crab_Melee) == 0 )
	{
		Melee();
		return 1;
	}
	else if (!isMelee&&ani->IsPlay(Crab_State::Crab_Range) == 0)
	{
		Range();
		return 1;
	}
	return 0;
}

bool Crab::Task_3_Wait()
{
	if (ani->IsPlay(Crab_State::Crab_Melee) || ani->IsPlay(Crab_State::Crab_Range))
	{
		state = prevState;
		return 0;
	}
	if (task_3_Time == 0.0f)//최초진입
	{
		task_3_Time = Time::Get()->Running();
		state = Crab_State::Crab_Idle;
	}
	if (Time::Get()->Running() - task_3_Time > 1.0f)//성공->리셋
	{
		task_3_Time = 0.0f;
		return 1;
	}
	return 0;
}

void Crab::AnimNotify_Shoot_Bubble()
{
	Vector2 pos; ani->GetTransform()->Position(&pos);
	Vector3 rot; ani->GetTransform()->RotationDegree(&rot);
	manager->Activate(Mission1_Projectile::M1_Crab, pos, isLeft*180.0f);
}

void Crab::AnimNotify_Act_Melee_Start()
{
	canMove = 0;
	isAct = 1;
	meleeCollider->SetPosition(ani->GetPosition());
}

void Crab::AnimNotify_Act_Melee_End()
{
	isAct = 0;
}

void Crab::AnimNotify_Act_End()
{
	canMove = 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Bubble::Bubble(UINT teamNumber, UINT poolIndex, float damage, ProjectileManager* manager, ColliderManager* colliderManager)
	: Bullet(teamNumber, poolIndex, damage, manager, colliderManager)
{
	collider = new Collider(Vector2(0, 0), Vector2(18, 18));
	collider->SetOffset(-1, 0);
	ani = new Animation();
	ani->AddSprite(L"monster/Crab/Crab_Bullet_bubble_Idle.png");
	ani->AddSprite(L"monster/Crab/Crab_Bullet_bubble_Pop.png");
	ani->SetPerSecond(15.0f);
	for (int i = 0; i < 2; i++)	ani->ReadFrameDatas(i);	

	teamNumb = teamNumber;
	health_Current = 1.0f;
	health_Max = 1.0f;

	colliderManager->AddHitObjects((IDamage*)this);
}

Bubble::~Bubble()
{
	
}

void Bubble::Update()
{
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

void Bubble::Render()
{
	if (!activated) return;
	if (ani->IsPlay() == 0)
	{
		Deactivate();
		return;
	}
	Bullet::Render();
	ani->Render();
	collider->Render();
}

void Bubble::Progress()
{
	if (!move)return;
	float y = Time::Get()->Running() - activationTime;
	float x = Time::Delta()*60.f;
	y = (4-y)*y*y*y*0.0015;
	Vector3 temp=ani->GetPosition();
	temp.x += x*forward.x;
	temp.y += y;
	ani->SetPosition(temp.x,temp.y);
	collider->SetPosition(temp);
}

void Bubble::Activate(float z)
{
	activationTime = Time::Get()->Running();
	forward.x = cos(z*Radian);
	forward.y = sin(z*Radian);
	activated = 1;
	move = 1;
}

void Bubble::Deactivate()
{
	activationTime = 0.0f;
	activated = 0;
	collider->SetScale(1.0f, 1.0f);
}

void Bubble::CollisionCheck()
{
	if (!move)return;
	colliderManager->IntersectObjects((Bullet*)this);
}

void Bubble::TakeDamage(float damage)
{
	health_Current -= damage;
	if (CheckDead())Pop();
}

bool Bubble::Intersect(Vector2 pos, Vector2 size)
{
	return false;
}

bool Bubble::CheckDead()
{
	if (health_Current <= 0.0f)return 1;
	return false;
}

Collider* Bubble::GetHitCollider()
{
	return collider;
}

void Bubble::Pop()
{
	colliderManager->DeleteHitObjetcs((IDamage*)this);
	if(ani->IsPlay(1)==0)ani->PlayOnceAndDisappear(1);
	activationTime = Time::Get()->Running();
	Vector2 temp = collider->GetScale();
	collider->SetScale(temp*2.0f);
	move = 0;
}
