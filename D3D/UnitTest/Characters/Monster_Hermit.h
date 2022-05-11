#pragma once
#include "Interfaces/ICharacter.h"
#include "Interfaces/IDamage.h"
#include "Objects/Projectile.h"
#include "Components/MovementComponent.h"

enum Hermit_State
{
	Hermit_Walk, Hermit_Fire,
	Hermit_Death, Hermit_Max
};

class Hermit : public IDamage, ICharacter
{
public:
	Hermit(ProjectileManager* manager, ColliderManager* colliderManager, Vector2 Position, Vector2 Scale = Vector2(1.4f, 1.4f));
	~Hermit();
	// ICharacter을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual bool IsGround() override { return 0; };
	virtual Vector2 GetPosition() override { return position; };

	// IDamage을(를) 통해 상속됨
	virtual void TakeDamage(float damage) override;
	virtual bool Intersect(Vector2 pos, Vector2 size) override { return 0; };
	virtual bool CheckDead() override;
	virtual Collider* GetHitCollider()override;
	
	void RenderCover();

	void SetScale(float x, float y);
	void SetScale(Vector2 pos);
	void SetPosition(float x, float y);
	void SetPosition(Vector2 pos);
	void SetLeft(float left);

	Vector2 GetScale() { Vector2 temp; ani->GetTransform(state)->Scale(&temp); return temp; }
	Vector2 GetRotation() { Vector2 temp; ani->GetTransform(state)->Rotation(&temp); return temp; }

	Hermit_State GetState() { return state; }
private:
	//class IDamage
	//float health_Current;
	//float health_Max;
	Animation* ani;
	Collider* hitCollider;
	Collider* meleeCollider;
	Collider* legCollider;
	ProjectileManager* manager;
	ColliderManager* colliderManager;
	MovementComponent* movement;

	Animation* waterSplash;
	Vector2 waterOffset;

	Animation* muzzleFlash;
	Vector2 muzzleOffset; Vector2 projectileOffset;

	Hermit_State prevState = Hermit_State::Hermit_Max;
	Hermit_State state = Hermit_State::Hermit_Walk;
	Vector2 position;
	Vector2 scale;
	int AIcount = 0;
	bool canAttack = 1;
	bool isMelee = 0;
	bool isRage = 0;
	bool moveRight = 0; float rightPoint=300.0f;
	bool moveLeft=0; float leftPoint;
	float size=45.0f;
	float moveSpeed = 200.0f;
	float defaultPerSecond = 10.0f;
	float perSecond = 10.0f;
	float prevTime;
	float coolTime = 5.0f;
private:
	void Init();
	void AnimInit();
	void ColliderInit();
	void AI();
	void FSM();
	void ColliderUpdate();
	void Walk();
	void Fire();
	void Death();
	void Rush();
	void Retreat();
	//task
	//notify
	//void AnimNotify_();
	void AnimNotify_AI();
	void AnimNotify_Melee_ColliderOn();
	void AnimNotify_Melee_ColliderOff();
	void AnimNotify_Fire();
	//imgui
	float x=0.0f;
	float y=0.0f;
};
/////////////////////////////////////FireBall
class Hermit_FireBall : public Bullet
{
public:
	Hermit_FireBall(UINT teamNumber, UINT poolIndex, float damage, ProjectileManager* manager, ColliderManager* colliderManager);
	~Hermit_FireBall();

	// Bullet을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void Progress() override;
	virtual void Activate(float z) override;
	virtual void Deactivate() override;
	virtual void CollisionCheck() override;
	virtual void Pop() override;

	void SetScale(float x,float y);
	void SetScale(Vector2 pos);
private:
	// Bullet을(를) 통해 상속됨
	//Animation* ani;
	//Collider* collider;
	//bool activated = 0;
	//UINT teamNumber = 0;

	Vector3 forward;
	float rotZ;

	Animation* trail[20]; UINT pivot=0;
	Animation* effect;
	float lifespan =0.0f;
	float activationTime =0.0f;
};
