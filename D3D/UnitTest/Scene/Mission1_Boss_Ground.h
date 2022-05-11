#pragma once
#include "Interfaces/IDamage.h"
class ColliderManager;
class Boss_Ground : public IDamage
{
public:
	Boss_Ground(UINT index,Vector2 scale, Vector2 position,ColliderManager* colliderManager, deque<Boss_Ground*>* pool);
	~Boss_Ground();
	void Update();
	void Move();
	void Render();

	// IDamage을(를) 통해 상속됨
	virtual void TakeDamage(float damage) override;
	virtual bool Intersect(Vector2 pos, Vector2 size)override { return 1; };
	virtual bool CheckDead() override;
	virtual Collider * GetHitCollider() override;

	void SetPosition(Vector3 position);
	void SetPosition(Vector2 position);
	void SetPosition(float x,float y);

	void RenderEffect();

	void AddPool();
	void GetBack();
	void SetHead() { isHead = !isHead; };

	Vector2 GetPosition() { return position; }
private:
	Sprite* normal;
	Sprite* head;
	Animation* effect;
	Collider* hitColldier;
	ColliderManager* colliderManager;
	deque<Boss_Ground*>* pool;
	Line* line;

	Vector2 scale;
	Vector2 position;
	Vector2 headOffset = Vector2(0.0f,0.0f);
	Vector2 vertexLeftOffset;
	Vector2 vertexRightOffset;

	bool isHead=0;
	float limit;
	void MakeLine(UINT index);
};