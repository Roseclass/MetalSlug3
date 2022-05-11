#pragma once

class ItemObj
{
public:
	ItemObj(class ColliderManager* manager,Vector2 position,Vector2 scale, Vector2 offset);
	~ItemObj();

	virtual void Init() = 0;
	virtual void AnimInit() = 0;
	virtual void ColliderInit() = 0;

	virtual void PosUpdate() = 0;
	virtual void AnimUpdate() = 0;

	void Update();
	void Render();
	void SetPosition(Vector2 position);
	void SetPosition(float x, float y);
	void SetScale(Vector2 scale);
	void SetScale(float x, float y);
	void SetRotation(Vector3 rotation);
	void SetRotation(float x, float y,float z);

	void Activate();
	void Deactivate();

	Collider* GetCollider() { return collider; }
	UINT GetItemNumb() { return itemNumb; }
protected:
	Animation* ani;
	Collider* collider;
	ColliderManager* manager;

	Vector2 position;
	Vector2 offset;
	Vector2 scale;

	UINT itemNumb = 0;

	bool activate = 1;

};