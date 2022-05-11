#include "stdafx.h"
#include "Player_Eri.h"
#include <iostream>

Eri::Eri(ProjectileManager* manager, ColliderManager* colliderManager, Vector2 Position, Vector2 Scale)
	:manager(manager)
	, colliderManager(colliderManager)
{
	teamNumb = 0;
	position = Position;
	//Load
	Init();
	SetScale(Scale);
	isLeft = 0;
}

Eri::~Eri()
{
	SafeDelete(upper);
	SafeDelete(lower);
	SafeDelete(full);
	SafeDelete(collider);
	SafeDelete(meleeAttack);
}

void Eri::Update()
{
	if (isProtected&&Time::Get()->Running() - protectionTime >= 3.0f)isProtected = 0;
	if (state==Eri_State::Eri_Death)
	{
		AnimUpdate();
		return;
	}
	prev.State = state;
	prev.State_Upper = state_Upper;
	prev.State_Lower = state_Lower;
	prev.State_Full = state_Full;
	prev.State_Weapon = state_Weapon;
	prev.isUpside = isUpside;
	ResetState();
	Controll();
	StateMachine();
	PosUpdate();
	AnimUpdate();
	ColliderUpdate();
}

void Eri::Render()
{
	if (state != Eri_State::Eri_Death)
	{
		collider->Render();
		meleeAttack->Render();
	}
	if (state_Anim == Eri_AnimState::Anim_Default)
	{
		upper->Render();
		lower->Render();
	}else full->Render();
}

bool Eri::IsGround()
{
	if (movement->IsGround(position)) return 1;
	return false;
}

void Eri::TakeDamage(float damage)
{
	if (isProtected)return;
	if (state == Eri_State::Eri_Death)return;
	cout << "damaged" << endl;
	health_Current -= damage;
	if (CheckDead()) Death();
}

bool Eri::CheckDead()
{
	if (health_Current <= 0.0f) return 1;
	return false;
}

Collider* Eri::GetHitCollider()
{
	return collider;
}

void Eri::SetScale(float x, float y)
{
	scale.x = x; scale.y = y;
	upper->SetScale(x, y);
	lower->SetScale(x, y);
	full->SetScale(x, y);
	collider->SetScale(x, y);
	meleeAttack->SetScale(x, y);
}

void Eri::SetScale(Vector2 scale)
{
	SetScale(scale.x, scale.y);
}

void Eri::SetPosition(float x, float y)
{
	position.x = x; position.y = y;
}

void Eri::SetPosition(Vector2 position)
{
	SetPosition(position.x, position.y);
}

void Eri::ChangeWeapon(UINT idx)
{
	state_Weapon = WeaponState(idx * WeaponState::Weapon_HeavyMG);
	AnimUpdate();
}

void Eri::Init()
{
	health_Max = 1.0f;
	health_Current = 1.0f;
	movement = new MovementComponent(this,colliderManager,300.0f,350.0f);
	LowerInit();
	UpperInit();
	FullInit();
	ColliderInit();
	upper->SetPerSecond(20.0f);
	lower->SetPerSecond(20.0f);
	bulletOffset.push_back(BulletOffset(-53.0f, 46.0f, 0.0f, 80.0f, 0.0f, 0.0f));
	bulletOffset.push_back(BulletOffset(-78.0f, 35.0f, 0.0f, 130.0f, 0.0f, 0.0f));
}

void Eri::LowerInit()
{
	//Idle,Walk,Jump
	lower = new Animation();
	lower->AddSprite(L"character/Eri/Eri_Lower_Idle.png");
	lowerOffset.push_back(Vector2(0.0f, 24.0f));
	upperOffset.push_back(Vector2(7.0f, 21.0f));
	lower->AddSprite(L"character/Eri/Eri_Lower_Walk.png");
	lowerOffset.push_back(Vector2(0.0f, 26.0f));
	upperOffset.push_back(Vector2(7.0f, 21.0f));
	lower->AddSprite(L"character/Eri/Eri_Lower_Jump_Idle.png");
	lowerOffset.push_back(Vector2(0.0f, 24.0f));
	upperOffset.push_back(Vector2(0.0f, 32.0f));
	lower->AddSprite(L"character/Eri/Eri_Lower_Jump_Walk.png");
	lowerOffset.push_back(Vector2(0.0f, 24.0f));
	upperOffset.push_back(Vector2(-12, 34.0f));
	for (int i = 0; i < Eri_LowerState::Lower_Max; i++)
	{
		lower->ReadFrameDatas(i);
	}
}

void Eri::UpperInit()
{
	//Idle,Walk,Melee,Range
	upper = new Animation();
	//pistol
	upper->AddSprite(L"character/Eri/Eri_Upper_Idle.png");
	upper->AddSprite(L"character/Eri/Eri_Upper_Walk.png");
	upper->AddSprite(L"character/Eri/Eri_Upper_Melee.png");
	upper->AddSprite(L"character/Eri/Eri_Upper_Range.png");
	upper->AddSprite(L"character/Eri/Eri_Upper_Jump_Idle.png");
	upper->AddSprite(L"character/Eri/Eri_Upper_Jump_Walk.png");
	upper->AddSprite(L"character/Eri/Eri_Upper_UpSide_Idle.png");
	upper->AddSprite(L"character/Eri/Eri_Upper_Upside_Range.png");
	//heavy
	upper->AddSprite(L"character/Eri/Eri_Heavy_Idle.png");
	upper->AddSprite(L"character/Eri/Eri_Heavy_Walk.png");
	upper->AddSprite(L"character/Eri/Eri_Heavy_Melee.png");
	upper->AddSprite(L"character/Eri/Eri_Heavy_Range.png");
	upper->AddSprite(L"character/Eri/Eri_Heavy_Jump_Idle.png");
	upper->AddSprite(L"character/Eri/Eri_Heavy_Jump_Walk.png");
	upper->AddSprite(L"character/Eri/Eri_Heavy_UpSide_Idle.png");
	upper->AddSprite(L"character/Eri/Eri_Heavy_Upside_Range.png");
	for (int i = 0; i < Eri_UpperState::Upper_Max; i++)
		upper->ReadFrameDatas(i);
	upper->SetStartNotify(Eri_UpperState::Upper_Range, 0, [this]() {this->AnimNotify_Pistol_Start(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Range, 5, [this]() {this->AnimNotify_Shoot_End(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Melee, 0, [this]() {this->AnimNotify_Act_Melee_Start(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Melee, 5, [this]() {this->AnimNotify_Act_Melee_End(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Upside_Range, 0, [this]() {this->AnimNotify_Pistol_Start(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Upside_Range, 5, [this]() {this->AnimNotify_Shoot_End(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Range_Heavy, 0, [this]() {this->AnimNotify_Heavymg_Start(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Range_Heavy, 2, [this]() {this->AnimNotify_Heavymg_Middle(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Range_Heavy, 4, [this]() {this->AnimNotify_Shoot_End(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Melee_Heavy, 0, [this]() {this->AnimNotify_Act_Melee_Start(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Melee_Heavy, 5, [this]() {this->AnimNotify_Act_Melee_End(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Upside_Range_Heavy, 0, [this]() {this->AnimNotify_Heavymg_Start(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Upside_Range_Heavy, 2, [this]() {this->AnimNotify_Heavymg_Middle(); });
	upper->SetStartNotify(Eri_UpperState::Upper_Upside_Range_Heavy, 3, [this]() {this->AnimNotify_Shoot_End(); });
}

void Eri::FullInit()
{
	full = new Animation();
	full->AddSprite(L"character/Eri/Eri_Full_Death.png");
	full->AddSprite(L"character/Eri/Eri_Full_Revive.png");
	for (int i = 0; i < Eri_FullState::Full_Max; i++)
		full->ReadFrameDatas(i);
	full->SetEndNotify(Eri_FullState::Full_Death, 15, [this]() {this->AnimNotify_Death_End(); });
	full->SetEndNotify(Eri_FullState::Full_Revive, 6, [this]() {this->AnimNotify_Revive_End(); });
}

void Eri::ColliderInit()
{
	collider = new Collider(position, Vector2(10.0f, 30.0f));
	collider->SetOffset(5.0f, 35.0f);
	colliderManager->AddHitObjects((IDamage*)this);
	meleeAttack = new Collider(position, Vector2(20.0f, 35.0f));
	meleeAttack->SetOffset(40.0f,50.0f);
}

void Eri::StateMachine()
{
	if (state_Anim == Eri_AnimState::Anim_Default)
	{
		UpperStateMachine();
		LowerStateMachine();
	}
	else FullStateMachine();
}

void Eri::ResetState()
{
	ChangeUpperState(Eri_UpperState::Upper_Idle);
	ChangeLowerState(Eri_LowerState::Lower_Idle);
	if (canChange)isUpside = 0;
}

void Eri::UpperStateMachine()
{
	if (prev.State_Upper == state_Upper&& prev.isUpside == isUpside&&upper->IsPlay(state_Upper)&&prev.State_Weapon==state_Weapon )return;
	int temp = state_Upper % Eri_UpperState::Upper_Idle_Heavy;
	if (temp == 0||temp==4)Upper_Idle();
	else if (temp == 1 || temp == 5)Upper_Walk();
	//else Upper_Attack();
}

void Eri::LowerStateMachine()
{
	if (prev.State_Lower == state_Lower/*&& state_Lower< Eri_LowerState::Rising*/)return;
	if (state_Lower == Eri_LowerState::Lower_Idle) Lower_Idle();
	else if (state_Lower == Eri_LowerState::Lower_Walk) Lower_Walk();
	else Lower_Jump();
}

void Eri::FullStateMachine()
{
	
}

void Eri::ChangeUpperState(Eri_UpperState state)
{
	if (!canChange)return;
	int temp = state;
	temp %= Eri_UpperState::Upper_Idle_Heavy;
	temp += state_Weapon;
	if (!IsGround())temp += 4;
	state_Upper = Eri_UpperState(temp);
}

void Eri::ChangeLowerState(Eri_LowerState state)
{
	int temp = state;
	if (!IsGround())temp += 2;
	state_Lower = Eri_LowerState(temp);
}

void Eri::Controll()
{
	if (canMove)
	{
		if (Keyboard::Get()->Press(VK_LEFT))
		{
			Left();
		}
		else if (Keyboard::Get()->Press(VK_RIGHT))
		{
			Right();
		}
		if (Keyboard::Get()->Press(VK_UP))
		{
			Up();
		}
		else if (Keyboard::Get()->Press(VK_DOWN))
		{
			Down();
		}
	}
	if (IsGround())
	{
		if (Keyboard::Get()->Down('C'))
		{
			Jump();
		}
	}
	if (canAttack)
	{
		if (Keyboard::Get()->Press('X'))
		{
			Upper_Attack();
		}
	}
	PosUpdate();
}

void Eri::Left()
{
	if (!isLeft)
	{
		isLeft = 1;
		meleeAttack->ReverseOffsetX();
		collider->ReverseOffsetX();
		upper->SetRotationDegree(0.0f, 180.0f, 0.0f);
		lower->SetRotationDegree(0.0f, 180.0f, 0.0f);
		full->SetRotationDegree(0.0f, 180.0f, 0.0f);
	}
	ChangeUpperState(Eri_UpperState::Upper_Walk);
	ChangeLowerState(Eri_LowerState::Lower_Walk);
	movement->MoveForward(position, isLeft);
}

void Eri::Right()
{
	if (isLeft)
	{
		isLeft = 0;
		meleeAttack->ReverseOffsetX();
		collider->ReverseOffsetX();
		upper->SetRotationDegree(0.0f, 0.0f, 0.0f);
		lower->SetRotationDegree(0.0f, 0.0f, 0.0f);
		full->SetRotationDegree(0.0f, 0.0f, 0.0f);
	}
	ChangeUpperState(Eri_UpperState::Upper_Walk);
	ChangeLowerState(Eri_LowerState::Lower_Walk);
	movement->MoveForward(position, isLeft);
}

void Eri::Up()
{
	isUpside = 1;
}

void Eri::Down()
{

}

void Eri::Jump()
{
	movement->Jump();
	if (state_Lower == Eri_LowerState::Lower_Idle)
	{
		ChangeUpperState(Eri_UpperState::Upper_Idle);
		ChangeLowerState(Eri_LowerState::Lower_Idle);
	}
	else
	{
		ChangeUpperState(Eri_UpperState::Upper_Walk);
		ChangeLowerState(Eri_LowerState::Lower_Walk);
	}
}

void Eri::PosUpdate()
{
	movement->Update(position);
	Vector2 temp = position;UINT itemnumb;
	if (colliderManager->IntersectItems(&temp, &itemnumb))ChangeWeapon(itemnumb);
	{
		isLeft ? temp.x -= lowerOffset[state_Lower].x*scale.x : temp.x += lowerOffset[state_Lower].x*scale.x;
		temp.y += lowerOffset[state_Lower].y*scale.y;
		lower->SetPosition(temp); 
	}
	{
		isLeft ? temp.x -= upperOffset[state_Lower].x*scale.x : temp.x += upperOffset[state_Lower].x*scale.x;
		UINT st = state_Upper % Eri_UpperState::Upper_Idle_Heavy;
		if (!IsGround() && (st == Eri_UpperState::Upper_Range||
			st == Eri_UpperState::Upper_Upside_Range||
			st == Eri_UpperState::Upper_Upside_Idle))
		{
			if (state_Lower == Eri_LowerState::Lower_Jump_Idle)
			{
				temp.y -= 3.0f * scale.y;
				isLeft ? temp.x -= 5.0f * scale.x : temp.x += 5.0f * scale.x;
			}
			else if (state_Lower == Eri_LowerState::Lower_Jump_Walk)
			{
				temp.y -= 6.0f* scale.y;
				isLeft ? temp.x -= 8.0f * scale.x : temp.x += 8.0f * scale.x;
			}
		}
		temp.y += upperOffset[state_Lower].y*scale.y;
		upper->SetPosition(temp);
	}
	full->SetPosition(temp + upperOffset[state_Full]);
	collider->SetPosition(position.x, position.y + size* scale.y);
	meleeAttack->SetPosition(position.x, position.y + size* scale.y);
}

void Eri::AnimUpdate()
{
	if (state_Anim == Eri_AnimState::Anim_Default)
	{
		upper->Update();
		lower->Update();
	}
	else full->Update();
}

void Eri::ColliderUpdate()
{
	if (colliderManager->IntersectObjects(meleeAttack, teamNumb,meleeDamage))
	{
		if (meleeDamage == 40.0f)meleeDamage = 0.0f;
		canMelee = 1;
	}
	else canMelee = 0;
	colliderManager->IntersectTriggers(&position);
	collider->Update();
	meleeAttack->Update();
}

void Eri::Upper_Idle()
{
	UINT temp = state_Weapon;
	if (isUpside)
	{
		state_Upper = (Eri_UpperState)(Eri_UpperState::Upper_Upside_Idle + state_Weapon);
		upper->PlayLoop(Eri_UpperState::Upper_Upside_Idle + state_Weapon);
	}
	else if (state_Upper == Eri_UpperState::Upper_Jump_Idle + state_Weapon)upper->PlayOnce(state_Upper);
	else upper->PlayLoop(state_Upper);
}

void Eri::Upper_Walk()
{
	if (isUpside)
	{
		state_Upper = (Eri_UpperState)(Eri_UpperState::Upper_Upside_Idle + state_Weapon);
		upper->PlayLoop(Eri_UpperState::Upper_Upside_Idle + state_Weapon);
	}
	else if(state_Upper%Eri_UpperState::Upper_Idle_Heavy == Eri_UpperState::Upper_Jump_Walk)
		upper->PlayOnce(state_Upper);
	else upper->PlayLoop(state_Upper);
}

void Eri::Upper_Attack()
{
	IsMelee();
	state = Eri_State::Eri_Attack;
	PosUpdate();
}

void Eri::IsMelee()
{
	if (canMelee)Melee();
	else Range();
}

void Eri::Melee()
{
	UINT temp = state_Weapon; 
	state_Upper = (Eri_UpperState)(Eri_UpperState::Upper_Melee + state_Weapon);
	upper->PlayMontage(Eri_UpperState::Upper_Melee);
}

void Eri::Range()
{
	UINT temp = state_Weapon; (Eri_UpperState)(Eri_UpperState::Upper_Range + state_Weapon);
	if (isUpside)
	{
		state_Upper = (Eri_UpperState)(Eri_UpperState::Upper_Upside_Range + state_Weapon);
		upper->PlayMontage(Eri_UpperState::Upper_Upside_Range + state_Weapon);
	}
	else
	{
		state_Upper = (Eri_UpperState)(Eri_UpperState::Upper_Range + state_Weapon);
		upper->PlayMontage(Eri_UpperState::Upper_Range + state_Weapon);
	}
}

void Eri::Lower_Idle()
{
	lower->PlayLoop(state_Lower);
}

void Eri::Lower_Walk()
{
	lower->PlayLoop(state_Lower);
}

void Eri::Lower_Jump()
{
	lower->PlayOnce(state_Lower);
}

void Eri::Rising()
{

}

void Eri::Falling()
{

}

void Eri::Death()
{
	state = Eri_State::Eri_Death;
	state_Full = Eri_FullState::Full_Death;
	state_Anim = Eri_AnimState::Anim_Full;
	full->Play(Eri_FullState::Full_Death);
	full->SetPerSecond(15.0f);
}

void Eri::AnimNotify_Pistol_Start()
{
	canAttack = 0;
	canChange = 0;
	float angle;
	if (isUpside) angle = 90.0f;
	else angle = -180.0f * isLeft;
	Vector2 pos=position;
	if (isUpside) pos+=Math::mul(bulletOffset[0].Up,scale);
	else if (isLeft) pos+= Math::mul(bulletOffset[0].Left,scale);
	else { pos.x -= bulletOffset[0].Left.x*scale.x; pos.y += bulletOffset[0].Left.y*scale.y; }
	if(!isUpside)manager->Activate(0, pos, angle);
	else manager->Activate(0, pos, angle,Vector3(0.0f,0.0f,angle));
}

void Eri::AnimNotify_Heavymg_Start()
{
	canAttack = 0;
	canChange = 0;
	float angle;
	if (isUpside) angle = 90.0f;
	else angle = -180.0f * isLeft;
	Vector2 pos = position;
	if (isUpside) pos += Math::mul(bulletOffset[1].Up+Vector2(3.0f,0.0f), scale);
	else if (isLeft) pos += Math::mul(bulletOffset[1].Left + Vector2(0.0f, 3.0f), scale);
	else { pos.x -= bulletOffset[1].Left.x*scale.x; pos.y += (bulletOffset[1].Left.y+3.0f)*scale.y; }
	if (!isUpside)manager->Activate(1, pos, angle);
	else manager->Activate(1, pos, angle, Vector3(0.0f, 0.0f, angle));
}

void Eri::AnimNotify_Heavymg_Middle()
{
	canAttack = 0;
	canChange = 0;
	float angle;
	if (isUpside) angle = 90.0f;
	else angle = -180.0f * isLeft;
	Vector2 pos = position;
	if (isUpside) pos += Math::mul(bulletOffset[1].Up-Vector2(3.0f, 0.0f), scale);
	else if (isLeft) pos += Math::mul(bulletOffset[1].Left - Vector2(0.0f, 3.0f), scale);
	else { pos.x -= bulletOffset[1].Left.x*scale.x; pos.y += (bulletOffset[1].Left.y-3.0f)*scale.y; }
	if (!isUpside)manager->Activate(1, pos, angle);
	else manager->Activate(1, pos, angle, Vector3(0.0f, 0.0f, angle));
}

void Eri::AnimNotify_Shoot_End()
{
	canAttack = 1;
	canChange = 1;
}

void Eri::AnimNotify_Act_Melee_Start()
{
	canAttack = 0;
	canChange = 0;
	meleeDamage = 40.0f;
}

void Eri::AnimNotify_Act_Melee_End()
{
	canAttack = 1;
	canChange = 1;
	meleeDamage = 0.0f;
	cout << "end\n";
}

void Eri::AnimNotify_Death_End()
{
	state_Full = Eri_FullState::Full_Revive;
	full->Play(Eri_FullState::Full_Revive);
	full->SetPerSecond(30.0f);
}

void Eri::AnimNotify_Revive_End()
{
	state = Eri_State::Eri_Idle;
	state_Full = Eri_FullState::Full_Max;
	state_Anim = Eri_AnimState::Anim_Default;
	UINT st = state_Weapon;
	prev.State_Upper= Eri_UpperState(st);
	state_Upper = Eri_UpperState(st);
	prev.State_Lower = Eri_LowerState::Lower_Idle;
	state_Lower = Eri_LowerState::Lower_Idle;
	canMove = 1;
	canAttack = 1;
	canChange = 1;
	canMelee = 0;
	isUpside = 0;
	health_Current = 1.0f;
	isProtected = 1;
	protectionTime = Time::Get()->Running();
}
