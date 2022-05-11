#pragma once
class ColliderManager;
class Wave
{
public:
	Wave(float x2, float y2, wstring directory, Vector2 scale,ColliderManager* colliderManager);
	~Wave();
	void Update();
	void RenderCover();
	void RenderBackground();
	void SetPlayerTarget(class ICharacter* target);
private:
	Animation* wave_Upper;
	Animation* wave_Lower;
	Animation* splash;
	ColliderManager* colliderManager;
	ICharacter* target;

	float left, right,top;
};
