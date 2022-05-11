#pragma once
#include "Interfaces/IDamage.h"
class LineEdit;
class ColliderManager;
class Rocket : public IDamage
{
public:
	Rocket(float x2,float y2, wstring directory,Vector2 scale, ColliderManager* colliderManager);
	~Rocket();
	void Update();
	void RenderCover();
	void RenderBackground();

	// IDamage을(를) 통해 상속됨
	virtual void TakeDamage(float damage) override;
	virtual bool Intersect(Vector2 pos, Vector2 size)override { return 1; };
	virtual bool CheckDead() override;
	virtual Collider * GetHitCollider() override;

	void AddEvent(function<void()> func);

private:
	Sprite* deafault;
	Sprite* deafault_hit;
	Sprite* deafault_cover;
	Sprite* broken;
	Sprite* broken_cover;
	Line* ground_Line;
	LineEdit* ground_LineEdit;
	Collider* hitColldier;
	Collider* blockingVolume;
	ColliderManager* colliderManager;

	bool Damaged = 0;
	function<void()> damagedEvent;
};