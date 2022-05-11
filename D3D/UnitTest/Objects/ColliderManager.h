#pragma once

class ColliderManager
{
public:
	ColliderManager();
	~ColliderManager();

	void AddBlockings(Collider* blockingVolume);
	void AddTriggers(Collider* triggerVolume, function<void()> func);
	//void AddProjectiles(class Bullet* projectile);
	void AddHitObjects(class IDamage* object);
	void AddGrounds(Line* groundLine);
	void AddCharacters(class ICharacter* character);
	void AddItems(class ItemObj* item);

	void DeleteBlockings(Collider* blockingVolume);
	void DeleteTriggers(Collider* triggerVolume);
	void DeleteHitObjetcs(class IDamage* object);
	void DeleteGrounds(Line* groundLine);
	void DeleteCharacters(class ICharacter* character);
	void DeleteItems(class ItemObj* item);

	bool FindPlayerPosition(ICharacter** target);
	bool FindPlayerPosition(Collider* collider, ICharacter** target);

	bool IntersectBlockings(Vector2* pPosition);
	bool IntersectTriggers(Vector2* pPosition);
	bool IntersectGrounds(Vector2* pPosition);//포지션 넣고 충돌되면 충돌위치로 바꿔서 리턴
	bool IntersectGrounds(Collider* collider);//콜라이더 넣고 충돌여부 확인
	bool IntersectGrounds(class Bullet* collider);//콜라이더 넣고 충돌여부 확인
	bool IntersectItems(Vector2* pPosition, UINT* itemNumber);
	bool IntersectObjects(Bullet* collider);
	bool IntersectObjects(Collider* collider, UINT teamNumb, float damage=0.0f);
	void IntersectAllObjects(Collider* collider, UINT teamNumb, float damage=0.0f);
private:
	//블로킹볼륨
	vector<Collider*> blockings;
	//트리거볼륨
	vector<pair<Collider*, function<void()>>> triggers;
	//아이템
	vector<class ItemObj*> items;
	//IDamage[teamNumb] 히트판정을 위함
	vector<vector<class IDamage*>> objects;
	//Ground
	vector<Line*> grounds;
	//캐릭터 콜라이더 상호작용을 위함
	vector<class ICharacter*> characters;

	bool Intersect(Collider* object, Collider* bullet);
	bool Intersect(ConvexHull* object, Collider* bullet);
};
