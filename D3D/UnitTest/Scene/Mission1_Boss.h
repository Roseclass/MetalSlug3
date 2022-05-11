#pragma once
#include "Systems/IExecute.h"
#include "Interfaces/IDamage.h"
class Hermit;
class Boss_Ground;
class ProjectileManager;
class Items_HeavyMG;
class ColliderManager;
class Eri;
class Mission1_Boss : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};
	virtual void SetMain(IExecute* main)override {};
	virtual void Start() override ;

private:
	//���
	Sprite* background;
	vector<Animation*> river_back;
	vector<Animation*> river_front;

	ProjectileManager* projectileManager;
	ColliderManager* colliderManager;

	Vector2 scale;//�۷ι� ������(��������/ȭ�������)
	Vector2 StartUv;//ī�޶� x�����ǿ����� ��ȭ�� uv
	Vector2 EndUv;

	deque<Boss_Ground*> pool;
	Hermit* hermit;
	Eri* player;
	Freedom* cam;
	Collider* trigger;
	Sprite* image; bool bImage=1;

	bool CamMove = 0;
	bool groundMove = 0;

	void CreateProjectileManager();
	void CreateColliderManager();
	void CreateRiver();
	void CreateGround();

	void GameStart();
};