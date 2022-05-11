#pragma once
#include "Interfaces/ICharacter.h"
#include "Interfaces/IDamage.h"
#include "Objects/Projectile.h"
#include "Objects/ColliderManager.h"
#include "Components/MovementComponent.h"

enum Eri_UpperState
{
	Upper_Idle,				Upper_Walk,				Upper_Melee,				Upper_Range,	
	Upper_Jump_Idle,		Upper_Jump_Walk,		Upper_Upside_Idle,			Upper_Upside_Range,
	Upper_Idle_Heavy,		Upper_Walk_Heavy,		Upper_Melee_Heavy,			Upper_Range_Heavy, 
	Upper_Jump_Idle_Heavy,	Upper_Jump_Walk_Heavy,	Upper_Upside_Idle_Heavy,	Upper_Upside_Range_Heavy,
	Upper_Max
};
enum Eri_LowerState
{
	Lower_Idle, Lower_Walk, Lower_Jump_Idle,Lower_Jump_Walk, Lower_Max
};
enum Eri_FullState
{
	Full_Death, Full_Revive,Full_Max
};

enum Eri_State
{
	Eri_Idle, Eri_Walk, Eri_Attack, Eri_Jump, Eri_Death, Eri_Max
};
enum Eri_AnimState
{
	Anim_Default, Anim_Full, Anim_Max
};
enum WeaponState
{
	Weapon_Pistol=0, Weapon_HeavyMG= Eri_UpperState::Upper_Idle_Heavy, Weapon_Max
};

class Eri : public IDamage, ICharacter
{
public:
	Eri(ProjectileManager* manager, ColliderManager* colliderManager,Vector2 Position, Vector2 Scale = Vector2(1, 1));
	~Eri();
	// ICharacter을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual bool IsGround() override;
	virtual Vector2 GetPosition() override { return position; };
	// IDamage을(를) 통해 상속됨
	virtual void TakeDamage(float damage) override;
	virtual bool Intersect(Vector2 pos, Vector2 size)override { return 0; };
	virtual bool CheckDead() override;
	virtual Collider* GetHitCollider()override;

	void SetScale(float x, float y);
	void SetScale(Vector2 scale);
	void SetPosition(float x, float y);
	void SetPosition(Vector2 position);
	Vector2* GetPositionPtr() { return &position; };

	void ChangeWeapon(UINT idx);

private:
	//class IDamage
	//float health_Current;
	//float health_Max;
	//class ICharacter
	//Vector2 position;//지면과 닿는점
	Animation* upper;
	Animation* lower;
	Animation* full;
	Collider* collider;
	Collider* meleeAttack;
	ProjectileManager* manager;
	ColliderManager* colliderManager;
	MovementComponent* movement;

	struct Prev
	{
		Eri_State State = Eri_State::Eri_Idle;
		Eri_UpperState State_Upper = Eri_UpperState::Upper_Idle;
		Eri_LowerState State_Lower = Eri_LowerState::Lower_Idle;
		Eri_FullState State_Full = Eri_FullState::Full_Max;
		WeaponState State_Weapon = WeaponState::Weapon_Pistol;
		bool isUpside = 0;
	}prev;
	Eri_State state = Eri_State::Eri_Idle;
	Eri_UpperState state_Upper = Eri_UpperState::Upper_Idle;
	Eri_LowerState state_Lower = Eri_LowerState::Lower_Idle;
	Eri_FullState state_Full = Eri_FullState::Full_Max;
	Eri_AnimState state_Anim = Eri_AnimState::Anim_Default;
	WeaponState state_Weapon = WeaponState::Weapon_Pistol;
	
	vector<Vector2> lowerOffset;//하체를 그려낼위치(지면점+오프셋)
	vector<Vector2> upperOffset;//상체를 그려낼위치(하체위치+오프셋)
	vector<Vector2> fullOffset;//전신을 그려낼위치(지면점 + 오프셋)
	struct BulletOffset
	{
		BulletOffset(float a, float b, float c, float d, float e, float f)
		{
			Left = Vector2(a, b);
			Up = Vector2(c, d);
			Down = Vector2(e, f);
		}
		Vector2 Left;
		Vector2 Up;
		Vector2 Down;
	};
	vector<BulletOffset> bulletOffset;//총알을 그려낼위치(지면점 + 오프셋)
	Vector2 scale;
	float size=0.0f;//position.y+size == 중점
	float meleeDamage = 0.0f;
	bool canMove = 1;//움직일수있는지?
	bool canAttack = 1;//공격이 가능한지?
	bool canMelee = 0;//근접공격이 가능한지?
	bool canChange = 1;//애니메이션이 바뀔수있는지?ex)위쪽보고쏘다가 조작을 안할때 쏘던건 마무리하고 바꿔야지
	bool isUpside = 0;
	bool isProtected = 0;float protectionTime = 0.0f;
private:
	//이니셜라이즈
	void Init();
	void LowerInit();
	void UpperInit();
	void FullInit();
	void ColliderInit();
	//애님스테이트머신	변화된 이넘에따라서 실행or유지
	void StateMachine();
	void ResetState();
	void UpperStateMachine();
	void LowerStateMachine();
	void FullStateMachine();
	void ChangeUpperState(Eri_UpperState state);
	void ChangeLowerState(Eri_LowerState state);

	//조작관련	이넘상태만 변화시켜줌
	void Controll();
	void Left();
	void Right();
	void Up();
	void Down();
	void Jump();

	//업데이트
	void PosUpdate();
	void AnimUpdate();
	void ColliderUpdate();

	//스테이트머신
	void Upper_Idle();
	void Upper_Walk();
	void Upper_Attack();
	void IsMelee();//근접공격할지말지
	void Melee();
	void Range();

	void Lower_Idle();
	void Lower_Walk();
	void Lower_Jump();
	void Rising();
	void Falling();

	void Death();

	//notify
	void AnimNotify_Pistol_Start();
	void AnimNotify_Heavymg_Start();
	void AnimNotify_Heavymg_Middle();
	void AnimNotify_Shoot_End();
	void AnimNotify_Act_Melee_Start();
	void AnimNotify_Act_Melee_End();
	void AnimNotify_Death_End();
	void AnimNotify_Revive_End();
};