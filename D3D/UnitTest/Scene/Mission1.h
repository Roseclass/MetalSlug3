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
	Freedom* cam;
	Collider* trigger;

	bool followCam = 0;

	void CreateProjectileManager();
	void CreateColliderManager();
	void CalcBackgroundGap();//배경과 땅 싱크 계산

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