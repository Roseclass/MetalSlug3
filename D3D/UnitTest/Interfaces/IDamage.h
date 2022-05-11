#pragma once

class IDamage
{
public:
	virtual void TakeDamage(float damage) = 0;
	virtual bool Intersect(Vector2 pos,Vector2 size) = 0;
	virtual bool CheckDead() = 0;
	virtual Collider* GetHitCollider() = 0;
	UINT GetTeamNumb() { return teamNumb; }
protected:
	UINT teamNumb;
	float health_Current;
	float health_Max;
};