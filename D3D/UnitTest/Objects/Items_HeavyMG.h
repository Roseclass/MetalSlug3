#pragma once
#include "Items.h"

class Items_HeavyMG : public ItemObj
{
public:
	Items_HeavyMG(ColliderManager* manager,Vector2 position,Vector2 scale, Vector2 offset);
	~Items_HeavyMG();

private:
	Vector2 position;
	Vector2 offset;
	Vector2 scale;

	float velocity = 0.0f;

	virtual void Init()override;
	virtual void AnimInit()override;
	virtual void ColliderInit()override;

	virtual void PosUpdate()override;
	virtual void AnimUpdate()override;

};