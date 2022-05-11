#pragma once
#include "Systems/IExecute.h"
class TutorialScene : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};
private:
	Sprite* backGround;
	Sprite* backGround_Sky;
	Sprite* foreGround;
	Sprite* alphabet[5];//A,B,C,D,X
	Sprite* cursor[3];//0:horizon 1:vertical 2:diagonal
	Animation* buttons[4];
	Animation* joyStick;

	Vector2 scale;
	UINT sequenceNumb = 0;
	UINT stickDirection = 4;
	wstring directory;

	void SetCursorDirection();
};
