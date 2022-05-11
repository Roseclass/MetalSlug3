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
	bool IntersectGrounds(Vector2* pPosition);//������ �ְ� �浹�Ǹ� �浹��ġ�� �ٲ㼭 ����
	bool IntersectGrounds(Collider* collider);//�ݶ��̴� �ְ� �浹���� Ȯ��
	bool IntersectGrounds(class Bullet* collider);//�ݶ��̴� �ְ� �浹���� Ȯ��
	bool IntersectItems(Vector2* pPosition, UINT* itemNumber);
	bool IntersectObjects(Bullet* collider);
	bool IntersectObjects(Collider* collider, UINT teamNumb, float damage=0.0f);
	void IntersectAllObjects(Collider* collider, UINT teamNumb, float damage=0.0f);
private:
	//���ŷ����
	vector<Collider*> blockings;
	//Ʈ���ź���
	vector<pair<Collider*, function<void()>>> triggers;
	//������
	vector<class ItemObj*> items;
	//IDamage[teamNumb] ��Ʈ������ ����
	vector<vector<class IDamage*>> objects;
	//Ground
	vector<Line*> grounds;
	//ĳ���� �ݶ��̴� ��ȣ�ۿ��� ����
	vector<class ICharacter*> characters;

	bool Intersect(Collider* object, Collider* bullet);
	bool Intersect(ConvexHull* object, Collider* bullet);
};
