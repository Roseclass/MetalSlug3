#pragma once
#include "Interfaces/ICharacter.h"
#include "Objects/ColliderManager.h"

class MovementComponent
{
public:
	MovementComponent(ICharacter* parent, ColliderManager* manager, float speed,float jumpPower);
	~MovementComponent();
	
	void Update(Vector2& position);
	void MoveForward(Vector2& position, bool IsLeft=0);//포지션을 받아오고, 포지션을 계산해서 리턴해줌
	void Jump();
	void GravityProcess(Vector2& position);
	void SetSpeed(float speed);
	void SetJumpPower(float jumpPower);
	bool IsGround(Vector2& position);//지면에 있는지 없는지 검사
private:
	ICharacter* parent;
	ColliderManager* manager;

	//bool isGround = 1;
	float speed;
	float jumpPower;
	float jumpVelocity=0.0f;
	float gravity = 0.49f;
};
