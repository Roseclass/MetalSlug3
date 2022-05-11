#pragma once
#include "Interfaces/ICharacter.h"
#include "Interfaces/IDamage.h"
#include "Objects/Projectile.h"
#include "Components/MovementComponent.h"

enum Crab_State
{
	Crab_Idle, Crab_Walk, Crab_Melee, Crab_Range, Crab_Floating, Crab_Death, Crab_Max
};

class Crab : public IDamage, ICharacter
{
public:
	Crab(bool Melee , ProjectileManager* manager, ColliderManager* colliderManager, Vector2 Position, Vector2 Scale = Vector2(1, 1));
	~Crab();
	// ICharacter을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual bool IsGround() override;
	virtual Vector2 GetPosition() override { return position; };

	// IDamage을(를) 통해 상속됨
	virtual void TakeDamage(float damage) override;
	virtual bool Intersect(Vector2 pos, Vector2 size) override { return 0; };
	virtual bool CheckDead() override;
	virtual Collider* GetHitCollider()override;
	
	void Reset(Vector2 Position);
	void StartEat();

	void SetScale(float x, float y);
	void SetScale(Vector2 pos);
	void SetRotation(float x, float y);
	void SetRotation(Vector2 pos);
	void SetPosition(float x, float y);
	void SetPosition(Vector2 pos);
	void SetTarget(ICharacter* player);
	void SetNoticeRange(Vector2 range);

	Vector2 GetScale() { Vector2 temp; ani->GetTransform(state)->Scale(&temp); return temp; }
	Vector2 GetRotation() { Vector2 temp; ani->GetTransform(state)->Rotation(&temp); return temp; }
private:
	//class IDamage
	//float health_Current;
	//float health_Max;
	Animation* ani;
	Collider* hitCollider;
	Collider* meleeCollider;
	Collider* noticeCollider;
	ProjectileManager* manager;
	ColliderManager* colliderManager;
	MovementComponent* movement;

	wstring directory;
	wstring extent;

	Crab_State prevState = Crab_State::Crab_Idle;
	Crab_State state = Crab_State::Crab_Idle;
	Vector2 position;//지면과 닿는점
	ICharacter* target;
	Vector2 scale;
	bool isMelee;
	bool isAct = 0;
	bool notice = 0;
	bool canMove = 1;
	int AIcount = 0;
	float meleeDamage = 100.0f;
	float size=45.0f;
	float moveSpeed = 200.0f;
	float melee = 100.0f;
	float range = 300.0f;
	float prevTime;
private:
	void Init();
	void ColliderInit();
	void AI();
	void FSM();
	void CheckLeft();
	void PosUpdate();
	void ColliderUpdate();
	void Idle();
	void Walk();
	void Melee();
	void Range();
	void Death();
	void Left();
	void Right();
	bool MoveTo(Vector2 pos,float range=20.0f);
	//task
	bool Task_0_Notice();
	bool Task_1_MoveTo();
	bool Task_2_Attack();
	bool Task_3_Wait(); float task_3_Time = 0.0f;
	//notify
	void AnimNotify_Shoot_Bubble();
	void AnimNotify_Act_Melee_Start();
	void AnimNotify_Act_Melee_End();
	void AnimNotify_Act_End();
	//imgui
	float x=0.0f;
	float y=0.0f;
};

class Bubble : public Bullet, IDamage
{
public:
	Bubble(UINT teamNumber, UINT poolIndex,float damage, ProjectileManager* manager, ColliderManager* colliderManager);
	~Bubble();
	// Bullet을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void Progress() override;
	virtual void Activate(float z) override;
	virtual void Deactivate() override;
	virtual void CollisionCheck() override;
	virtual void Pop() override;
	

	// IDamage을(를) 통해 상속됨
	virtual void TakeDamage(float damage) override;
	virtual bool Intersect(Vector2 pos, Vector2 size) override;
	virtual bool CheckDead() override;
	virtual Collider* GetHitCollider()override;

protected:
	// Bullet을(를) 통해 상속됨
	//Animation* ani;
	//Collider* collider;
	//bool activated = 0;
	//UINT teamNumber = 0;

	// IDamage을(를) 통해 상속됨
	//float health_Current;
	//float health_Max;

	
private:
	Vector3 forward;

	float activationTime = 0.0f;
	float lifespan = 3.0f;
	bool move = 0;
};