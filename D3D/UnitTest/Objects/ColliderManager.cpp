#include "stdafx.h"
#include "ColliderManager.h"
#include "Interfaces/IDamage.h"
#include "Objects/Projectile.h"
#include "Objects/Items.h"
#include "Characters/Player_Eri.h"
#include "Interfaces/ICharacter.h"

ColliderManager::ColliderManager()
{

}

ColliderManager::~ColliderManager()
{

}

void ColliderManager::AddBlockings(Collider * blockingVolume)
{
	blockings.push_back(blockingVolume);
}

void ColliderManager::AddTriggers(Collider * triggerVolume, function<void()> func)
{
	UINT size = triggers.size();
	for (UINT i = 0; i < size; i++)
	{
		if (triggers[i].first == triggerVolume)
		{
			triggers[i].second=func;
			return;
		}
	}
	triggers.push_back({ triggerVolume ,func });
}

void ColliderManager::AddHitObjects(IDamage * object)
{
	UINT teamNumb = object->GetTeamNumb();
	while (teamNumb >= objects.size())
		objects.push_back(vector<IDamage*>());
	objects[teamNumb].push_back(object);
}

void ColliderManager::AddGrounds(Line * groundLine)
{
	grounds.push_back(groundLine);
}

void ColliderManager::AddCharacters(ICharacter * character)
{

}

void ColliderManager::AddItems(ItemObj * item)
{
	items.push_back(item);
}

void ColliderManager::DeleteBlockings(Collider * blockingVolume)
{
	for (UINT i = 0; i < blockings.size(); i++)
		if (blockingVolume == blockings[i])
		{
			blockings.erase(blockings.begin() + i);
			return;
		}
}

void ColliderManager::DeleteTriggers(Collider * triggerVolume)
{
	UINT size = triggers.size();
	for (UINT i = 0; i < size; i++)
	{
		if (triggers[i].first == triggerVolume)
		{
			triggers.erase(triggers.begin() + i);
			return;
		}
	}
}

void ColliderManager::DeleteHitObjetcs(IDamage * object)
{
	UINT teamNumb = object->GetTeamNumb();
	UINT size = objects.size();
	for (UINT i=0; i<objects[teamNumb].size(); i++)
	{
		if (object == objects[teamNumb][i])
		{
			objects[teamNumb].erase(objects[teamNumb].begin() + i);
			return;
		}
	}
}

void ColliderManager::DeleteGrounds(Line * groundLine)
{
	for (UINT i=0;i<grounds.size(); i++)
		if (groundLine == grounds[i])
		{
			grounds.erase(grounds.begin()+i);
			return;
		}	
}

void ColliderManager::DeleteCharacters(ICharacter * character)
{
}

void ColliderManager::DeleteItems(ItemObj * item)
{
	for (UINT i = 0; i < items.size(); i++)
		if (item == items[i])
		{
			items.erase(items.begin() + i);
			return;
		}
}

bool ColliderManager::FindPlayerPosition(ICharacter ** target)
{
	for (IDamage* i : objects[0])
	{
		Eri* temp = dynamic_cast<Eri*>(i);
		if (temp == nullptr)continue;
		*target = (ICharacter*)temp;
		return 1;
	}
	return false;
}

bool ColliderManager::FindPlayerPosition(Collider * collider,ICharacter** target)
{
	for (IDamage* i : objects[0])
	{
		Eri* temp = dynamic_cast<Eri*>(i);
		if (temp == nullptr)continue;
		if (!Intersect(temp->GetHitCollider(), collider))continue;
		*target = (ICharacter*)temp;
		return 1;		
	}
	return false;
}

bool ColliderManager::IntersectBlockings(Vector2* pPosition)
{
	for (Collider* i : blockings)
	{
		if (i->Intersect(*pPosition))
		{
			if(i->GetLeft()<= pPosition->x&&pPosition->x<=i->GetRight())
			{
				if (abs(i->GetLeft() - pPosition->x) < abs(i->GetRight() - pPosition->x)) pPosition->x = i->GetLeft() - 0.1f;
				else pPosition->x = i->GetLeft() + 0.1f;
			}
			if (i->GetBottom() <= pPosition->x&&pPosition->x <= i->GetTop())
			{			
				/*if (abs(i->GetBottom() - pos.y) < abs(i->GetTop() - pos.y)) pos.x = i->GetBottom() - 0.1f;
				else */	pPosition->y = i->GetTop() + 0.1f;
			}
			return 1;
		}
	}
	return false;
}

bool ColliderManager::IntersectTriggers(Vector2 * pPosition)
{
	bool result = 0;
	UINT size = triggers.size();
	for (UINT i = 0; i<size; i++)
	{
		if (triggers[i].first->Intersect(*pPosition))
		{
			triggers[i].second();
			result = 1;
		}
	}
	return result;
}

bool ColliderManager::IntersectGrounds(Vector2 * pPosition)
{
	vector<float> vec;
	for (Line* i : grounds)
	{
		Vector2 s, e;
		i->GetLine(*pPosition, &s, &e);
		float a = (pPosition->x - s.x) /(e.x - s.x);
		if (a > 1.0f)continue;
		if (s.x == 123456789)continue;
		float y = s.y*(1.0f - a) + e.y*a;
		vec.push_back(y);
	}
	if (!vec.size())return 0;
	if (vec.size() == 2)
		vec.size();
	sort(vec.begin(), vec.end(),greater<float>());
	for (float i : vec)
	{
		if (abs(pPosition->y - i) <= 1.0f)
		{
			pPosition->y = i;
			return 1;
		}
	}
	return false;
}

bool ColliderManager::IntersectGrounds(Collider * collider)
{
	return false;
}

bool ColliderManager::IntersectGrounds(Bullet * collider)
{
	Vector2 pos,size; collider->GetCollider()->GetTransform()->Position(&pos);
	size = collider->GetCollider()->GetSize();
	float x1, x2, y1, y2;
	x1 = pos.x + size.x; x2 = pos.x - size.x;
	y1 = pos.y + size.y; y2 = pos.y - size.y;
	if (IntersectGrounds(new Vector2(x1, y1)) ||
		IntersectGrounds(new Vector2(x1, y2)) || 
		IntersectGrounds(new Vector2(x2, y1)) || 
		IntersectGrounds(new Vector2(x2, y2)))
	{
		collider->Pop();
		return 1;
	}
	return false;
}

bool ColliderManager::IntersectObjects(Bullet * collider)
{
	UINT teamNumb = collider->GetTeamNumb();
	for (UINT i=0; i<objects.size(); i++)
	{
		if (teamNumb--==0)continue;
		for (IDamage* object : objects[i])
		{
			if (Intersect(object->GetHitCollider(), collider->GetCollider()))
			{
				object->TakeDamage(collider->GetDamage());
				collider->Pop();
				return 1;
			}
		}
	}
	return false;
}

bool ColliderManager::IntersectObjects(Collider * collider,UINT teamNumb,float damage)
{
	for (UINT i = 0; i < objects.size(); i++)
	{
		if (!teamNumb--)continue;
		for (IDamage* object : objects[i])
		{
			if (Intersect(object->GetHitCollider(), collider))
			{
				if(damage!=0.0f)object->TakeDamage(damage);
				return 1;
			}
		}
	}
	return false;
}

bool ColliderManager::IntersectItems(Vector2* pPosition,UINT* itemNumber)
{
	float x = pPosition->x;
	float y = pPosition->y;
	for (ItemObj* i : items)
	{
		if ((x > i->GetCollider()->GetLeft()) == (x > i->GetCollider()->GetRight())) continue;
		if ((y > i->GetCollider()->GetBottom()) == (y > i->GetCollider()->GetTop())) continue;
		*itemNumber = i->GetItemNumb();
		i->Deactivate();
		return 1;
	}
	return false;
}

void ColliderManager::IntersectAllObjects(Collider * collider, UINT teamNumb, float damage)
{
	for (UINT i = 0; i < objects.size(); i++)
	{
		if (!teamNumb--)continue;
		for (IDamage* object : objects[i])
		{
			if (Intersect(object->GetHitCollider(), collider))
			{
				if (damage != 0.0f)
					object->TakeDamage(damage);
			}
		}
	}
}

bool ColliderManager::Intersect(Collider* object, Collider* bullet)
{
	ConvexHull* temp = dynamic_cast<ConvexHull*>(object);
	if (temp != nullptr) return Intersect(temp, bullet);
	Vector2 op; object->GetTransform()->Position(&op); Vector2 os = object->GetSize();
	Vector2 bp; bullet->GetTransform()->Position(&bp); Vector2 bs = bullet->GetSize();
	bool x = 0, y = 0;
	if (abs(op.x - bp.x) <= (os.x + bs.x))x = 1;
	if (abs(op.y - bp.y) <= (os.y + bs.y))y = 1;
	return x&&y;
}

bool ColliderManager::Intersect(ConvexHull * object, Collider * bullet)
{
	vector<Vector3>* temp = object->GetVertices();
	Vector2 op; object->GetTransform()->Position(&op);
	Vector2 bp; bullet->GetTransform()->Position(&bp); Vector2 bs = bullet->GetSize();
	Vector2 A, B, C; int count;
	C = Vector2(bp.x - bs.x, bp.y + bs.y);//ÁÂ»ó
	count = 0;
	for (int j = 0; j < temp->size()-1; j++)
	{
		A = Vector2(op.x + temp->at(j).x, op.y + temp->at(j).y);
		B = Vector2(op.x + temp->at(j+1).x, op.y + temp->at(j+1).y);
		if ((A.y > C.y) == (B.y > C.y))continue;
		float atX = (B.x - A.x)*(C.y - A.y) / (B.y - A.y) + A.x;
		if (C.x < atX)count++;
	}
	if (count % 2)return 1;
	C = Vector2(bp.x - bs.x, bp.y - bs.y);//ÁÂÇÏ
	count = 0;
	for (int j = 0; j < temp->size() - 1; j++)
	{
		A = Vector2(op.x + temp->at(j).x, op.y + temp->at(j).y);
		B = Vector2(op.x + temp->at(j + 1).x, op.y + temp->at(j + 1).y);
		if ((A.y > C.y) == (B.y > C.y))continue;
		float atX = (B.x - A.x)*(C.y - A.y) / (B.y - A.y) + A.x;
		if (C.x < atX)count++;
	}
	if (count % 2)return 1;
	C = Vector2(bp.x + bs.x, bp.y - bs.y);//¿ìÇÏ
	count = 0;
	for (int j = 0; j < temp->size() - 1; j++)
	{
		A = Vector2(op.x + temp->at(j).x, op.y + temp->at(j).y);
		B = Vector2(op.x + temp->at(j + 1).x, op.y + temp->at(j + 1).y);
		if ((A.y > C.y) == (B.y > C.y))continue;
		float atX = (B.x - A.x)*(C.y - A.y) / (B.y - A.y) + A.x;
		if (C.x < atX)count++;
	}
	if (count % 2)return 1;
	C = Vector2(bp.x + bs.x, bp.y + bs.y);//¿ì»ó
	count = 0;
	for (int j = 0; j < temp->size() - 1; j++)
	{
		A = Vector2(op.x + temp->at(j).x, op.y + temp->at(j).y);
		B = Vector2(op.x + temp->at(j + 1).x, op.y + temp->at(j + 1).y);
		if ((A.y > C.y) == (B.y > C.y))continue;
		float atX = (B.x - A.x)*(C.y - A.y) / (B.y - A.y) + A.x;
		if (C.x < atX)count++;
	}
	if (count % 2)return 1;
	return 0;
}
