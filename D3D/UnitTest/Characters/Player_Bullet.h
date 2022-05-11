#pragma once
#include "Objects/Projectile.h"

class Bullet_Pistol: public Bullet
{
public:
	Bullet_Pistol(UINT teamNumber, UINT poolIndex, float damage, ProjectileManager* manager, ColliderManager* colliderManager);
	~Bullet_Pistol();
	// Bullet��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render() override;
	virtual void Progress() override;
	virtual void Activate(float z) override;
	virtual void Deactivate() override;
	virtual void CollisionCheck() override;
	virtual void Pop() override;	//������Ʈ�� ���Ϳ� ������ ������������ ������,�ٴ��̸� ���� �����͹������� �����Ը��鿹��


protected:
	// Bullet��(��) ���� ��ӵ�
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
	// Bullet��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render() override;
	virtual void Progress() override;
	virtual void Activate(float z) override;
	virtual void Deactivate() override;
	virtual void CollisionCheck() override;
	virtual void Pop() override;	//������Ʈ�� ���Ϳ� ������ ������������ ������,�ٴ��̸� ���� �����͹������� �����Ը��鿹��


protected:
	// Bullet��(��) ���� ��ӵ�
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