#pragma once

class ICharacter
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual bool IsGround() = 0;
	virtual Vector2 GetPosition() = 0;
protected:
	Vector2 position;
	bool isLeft;
};