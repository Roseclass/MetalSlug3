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
	//���
	Sprite* background;//�������� ȥ�� �ٸ�
	Sprite* ground;
	float gap, backstart, Groundstart, x;//���,�� �Ÿ����

	ProjectileManager* projectileManager;
	ColliderManager* colliderManager;

	//������Ʈ ~�������
	//�ĵ�
	Wave* wave;

	//���� ///0:�밡����,1:���� ���� ���� ���,2:Ŀ��,3:����,4:������
	Sprite* fish[5];
	//����
	Rocket* rocket;
	//���ĵ� ��Ʈ
	Boat* boat;
	//���ļ�
	Ship* ship;

	Line* ground_Line;//�ٴ�
	LineEdit* ground_LineEdit;//�ٴ� ������

	Vector2 scale;//�۷ι� ������(�׶��� ������)
	wstring directory;//_Textures/ ������ ���

	Crab* monster_Crab[5];
	Eri* player;
	Bubble* bubble;

	bool followCam = 1;

	void CreateProjectileManager();
	void CreateColliderManager();
	void CalcBackgroundGap();//���� �� ��ũ ���
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

	// IDamage��(��) ���� ��ӵ�
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

	// IDamage��(��) ���� ��ӵ�
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
