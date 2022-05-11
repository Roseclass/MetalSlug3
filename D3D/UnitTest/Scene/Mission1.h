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
	virtual void SetMain(IExecute* main)override {	this->main = dynamic_cast<Main*>(main);};
	virtual void Start() override { Event_Start(); };

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
	Freedom* cam;
	Collider* trigger;

	bool followCam = 0;

	void CreateProjectileManager();
	void CreateColliderManager();
	void CalcBackgroundGap();//���� �� ��ũ ���

	void Event_Start();
	void Event_Fish();
	void Event_Phase_Rocket();
	void Event_Phase_Boat();
	void Event_Phase_Ship();
	void Event_Phase_NextLevel();

private:
	Main* main;
	void LoadNextScene();
};