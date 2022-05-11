#pragma once
#include "Systems/IExecute.h"
#include "Interfaces/IDamage.h"
class Wave;
class Rocket;
class Boat;
class Ship;
class LineEdit;
class Crab;
class Bubble;
class ProjectileManager;
class ColliderManager;
class Eri;
class Mission1 : public IExecute
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
	//배경
	Sprite* background;//스케일이 혼자 다름
	Sprite* ground;
	float gap, backstart, Groundstart, x;//배경,땅 거리계산

	ProjectileManager* projectileManager;
	ColliderManager* colliderManager;

	//오브젝트 ~등장순서
	//파도
	Wave* wave;

	//생선 ///0:대가리뼈,1:몸통 파인 생선 배경,2:커버,3:생선,4:생선벼
	Sprite* fish[5];
	//로켓
	Rocket* rocket;
	//난파된 보트
	Boat* boat;
	//난파선
	Ship* ship;

	Line* ground_Line;//바닥
	LineEdit* ground_LineEdit;//바닥 에디터

	Vector2 scale;//글로벌 스케일(그라운드 스케일)
	wstring directory;//_Textures/ 이후의 경로

	Crab* monster_Crab[5];
	Eri* player;
	Bubble* bubble;

	bool followCam = 1;

	void CreateProjectileManager();
	void CreateColliderManager();
	void CalcBackgroundGap();//배경과 땅 싱크 계산
};
////////////////Wave////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////Rocket//////////////////////////////////////////////////////////////////////////////////////////////////////////
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

private:
	Sprite* deafault;
	Sprite* deafault_hit;
	Sprite* deafault_cover;
	Sprite* broken;
	Sprite* broken_cover;
	Line* ground_Line;
	LineEdit* ground_LineEdit;
	Collider* hitColldier;
	ColliderManager* colliderManager;

	bool Damaged = 0;

};
////////////////WreckedBoat//////////////////////////////////////////////////////////////////////////////////////////////////////////
class Boat : public IDamage
{
public:
	Boat(float x2, float y2, wstring directory, Vector2 scale, ColliderManager* colliderManager);
	~Boat();
	void Update();
	void RenderCover();
	void RenderBackground();

	// IDamage을(를) 통해 상속됨
	virtual void TakeDamage(float damage) override;
	virtual bool Intersect(Vector2 pos, Vector2 size)override { return 1; };
	virtual bool CheckDead() override;
	virtual Collider * GetHitCollider() override;

private:
	Sprite* deafault;
	Sprite* broken;
	Sprite* broken_cover;
	Line* ground;
	Collider* hitColldier;
	ColliderManager* colliderManager;
	bool Damaged = 0;

};
////////////////WreckedShip//////////////////////////////////////////////////////////////////////////////////////////////////////////
class Ship
{
public:
	Ship(float x2, float y2, wstring directory, Vector2 scale);
	~Ship();
	void Update();
	void RenderCover();
	void RenderBackground();
private:
	Animation* back;
	Animation* door;
	Sprite* cover;
	Sprite* cover_End;
	bool EvenetEnd = 0;
	void Event();

	float x, y;
	float sx, sy;
	float a=0.0f, b = 0.0f;
};
