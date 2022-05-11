#pragma once
#include "Systems/IExecute.h"
#include "Interfaces/IDamage.h"
enum Mission1_Projectile
{
	M1_Pistol, M1_HeavyMG, M1_Crab, M1_Max
};

class ProjectileManager;
class ColliderManager;
class Bullet
{
public:
	Bullet(UINT teamNumber,UINT poolIndex, float damage,ProjectileManager* manager, ColliderManager* colliderManager);
	~Bullet();
	virtual void Update();
	virtual void Render();
	virtual void Progress()=0;
	virtual void Activate(float z)=0;
	virtual void Deactivate()=0;
	virtual void CollisionCheck()=0;
	virtual void Pop()=0;

	void SetScale(float x, float y);
	void SetScale(Vector2 pos);
	void SetRotation(float x, float y,float z);
	void SetRotation(Vector3 pos);
	void SetPosition(float x, float y);
	void SetPosition(Vector2 pos);
	void SetTeamNumber(UINT teamNumber);
	void Destroy();

	bool Intersect(float x, float y);
	bool Intersect(Vector2 pos);
	bool Intersect(IDamage* objects);
	bool IsActivated() { return activated; }

	float GetDamage() { return damage; }
	UINT GetTeamNumb() { return teamNumber; }
	Animation* GetAnimation() { return ani; }
	Collider* GetCollider() { return collider; }
protected:
	Animation* ani;
	Collider* collider;
	ProjectileManager* manager;
	ColliderManager* colliderManager;

	Vector2 scale = Vector2(1.0f, 1.0f);
	bool debug = 0;
	bool activated = 0;
	UINT teamNumber = 0;
	UINT poolIndex;
	float damage;
};

class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();
	void Update();
	void Render();
	void Add(UINT poolSize);
	void AddBullet(UINT poolIndex, Bullet* input);
	void Activate(UINT poolIdx,Vector2 position, float z, Vector3 rotation=Vector3(0.0f, 0.0f, 0.0f));

private:
	vector<UINT> pivots; //new Ω√¿€¡°
	vector<UINT> size;
	vector<vector<Bullet*>> pools;

	//imgui
	int poolIndex = 0;
};
