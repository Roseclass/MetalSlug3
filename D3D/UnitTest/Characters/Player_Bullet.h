#pragma once
#include "Objects/Projectile.h"

class Bullet_Pistol: public Bullet
{
public:
	Bullet_Pistol(UINT teamNumber, UINT poolIndex, float damage, ProjectileManager* manager, ColliderManager* colliderManager);
	~Bullet_Pistol();
	// Bullet을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void Progress() override;
	virtual void Activate(float z) override;
	virtual void Deactivate() override;
	virtual void CollisionCheck() override;
	virtual void Pop() override;	//오브젝트나 몬스터에 닿으면 수직방향으로 터지고,바닥이면 라인 업벡터방향으로 터지게만들예정


protected:
	// Bullet을(를) 통해 상속됨
	//Animation* ani;
	//Collider* collider;
	//bool debug = 0;
	//bool activated = 0;
	//UINT teamNumber = 0;

private:
	Vector3 forward;

	float activationTime = 0.0f;
	float lifespan = 3.0f;
	bool move = 0;
};

class Bullet_HeavyMG : public Bullet
{
public:
	Bullet_HeavyMG(UINT teamNumber, UINT poolIndex, float damage, ProjectileManager* manager, ColliderManager* colliderManager);
	~Bullet_HeavyMG();
	// Bullet을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void Progress() override;
	virtual void Activate(float z) override;
	virtual void Deactivate() override;
	virtual void CollisionCheck() override;
	virtual void Pop() override;	//오브젝트나 몬스터에 닿으면 수직방향으로 터지고,바닥이면 라인 업벡터방향으로 터지게만들예정


protected:
	// Bullet을(를) 통해 상속됨
	//Animation* ani;
	//Collider* collider;
	//bool debug = 0;
	//bool activated = 0;
	//UINT teamNumber = 0;

private:
	Vector3 forward;

	float activationTime = 0.0f;
	float lifespan = 3.0f;
	bool move = 0;
};