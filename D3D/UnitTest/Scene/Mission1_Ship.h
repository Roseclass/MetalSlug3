#pragma once
class LineEdit;
class ColliderManager;
class Ship
{
public:
	Ship(float x2, float y2, wstring directory, Vector2 scale, ColliderManager* colliderManager);
	~Ship();
	void Update();
	void RenderCover();
	void RenderBackground();
	void Event();
private:
	Animation* back;
	Animation* door;
	Sprite* cover;
	Line* ground_Line;
	Line* ground_Line_Deck;
	LineEdit* ground_LineEdit;
	LineEdit* ground_LineEdit_Deck;
	ColliderManager* colliderManager;

	//Sprite* cover_End;
	bool EvenetEnd = 0;

	float x, y;
	float sx, sy;
	float a=0.0f, b = 0.0f;
};
