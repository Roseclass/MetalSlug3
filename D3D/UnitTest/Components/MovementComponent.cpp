#include "stdafx.h"
#include "MovementComponent.h"
#include "Interfaces/ICharacter.h"
#include <iostream>
MovementComponent::MovementComponent(ICharacter* parent, ColliderManager* manager, float speed, float jumpPower)
	:parent(parent)
	, manager(manager)
	, speed(speed)
	, jumpPower(jumpPower)
{

}

MovementComponent::~MovementComponent()
{

}

void MovementComponent::Update(Vector2 & position)
{
	Vector2 pos = position;
	if (manager->IntersectGrounds(&pos) && jumpVelocity <= 0.0f)
	{
		position = pos;
		jumpVelocity = 0.0f;
	}
	else GravityProcess(position);
	pos = position;
	manager->IntersectBlockings(&pos);
	position = pos;
}

void MovementComponent::MoveForward(Vector2& position, bool IsLeft)
{
	if (IsLeft)	position.x -= Time::Delta()*speed;
	else position.x += Time::Delta()*speed;
}

void MovementComponent::Jump()
{
	jumpVelocity=jumpPower;
}

void MovementComponent::GravityProcess(Vector2& position)
{
	jumpVelocity -= gravity;
	position.y += jumpVelocity * Time::Delta();
}

bool MovementComponent::IsGround(Vector2 & position)
{
	Vector2 pos = position;
	if (jumpVelocity!=0.0f)return 0;
	return 1;
}

void MovementComponent::SetSpeed(float speed)
{
	this->speed = speed;
}

void MovementComponent::SetJumpPower(float jumpPower)
{
	this->jumpPower = jumpPower;
}
