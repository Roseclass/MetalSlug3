#include "stdafx.h"
#include "Projectile.h"
#include "Objects/ColliderManager.h"

Bullet::Bullet(UINT teamNumber, UINT poolIndex, float damage,ProjectileManager* manager, ColliderManager* colliderManager)
	: teamNumber(teamNumber)
	, poolIndex(poolIndex)
	, damage(damage)
	, manager(manager)
	, colliderManager(colliderManager)
{

}

Bullet::~Bullet()
{
	SafeDelete(ani);
	SafeDelete(collider);
}

void Bullet::Update()
{
	ani->Update();
	collider->Update();
}

void Bullet::Render()
{
	if (debug)collider->Render();
}

void Bullet::SetScale(float x, float y)
{
	ani->SetScale(x,y);
	collider->SetScale(x, y);
	scale = Vector2(x, y);
}

void Bullet::SetScale(Vector2 pos)
{
	ani->SetScale(pos);
	collider->SetScale(pos);
	scale = pos;
}

void Bullet::SetRotation(float x, float y, float z)
{
	ani->SetRotationDegree(x, y,z);
	collider->SetRotationDegree(x, y, z);
}

void Bullet::SetRotation(Vector3 pos)
{
	ani->SetRotationDegree(pos);
	ani->Update();
	collider->SetRotationDegree(pos);
	collider->Update();
}

void Bullet::SetPosition(float x, float y)
{
	ani->SetPosition(x, y);
	ani->Update();
	collider->SetPosition(x, y);	
	collider->Update();
}

void Bullet::SetPosition(Vector2 pos)
{
	ani->SetPosition(pos);
	collider->SetPosition(pos);
}

void Bullet::SetTeamNumber(UINT teamNumber)
{
	this->teamNumber = teamNumber;
}

void Bullet::Destroy()
{
	SafeDelete(ani);
	SafeDelete(collider);

}

bool Bullet::Intersect(float x, float y)
{
	return collider->Intersect(x, y);
}

bool Bullet::Intersect(Vector2 pos)
{
	return Intersect(pos.x, pos.y);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ProjectileManager::ProjectileManager()
{

}

ProjectileManager::~ProjectileManager()
{
	for (int i = 0; i < pools.size(); i++)
	{
		for (int j = 0; j < pools[i].size(); j++)
			pools[i][j]->Destroy();
	}
}

void ProjectileManager::Update()
{
	ImGui::Begin("Projectile");
	ImGui::InputInt("PoolIndex", &poolIndex);
	poolIndex %= pools.size();
	if (ImGui::Button("Activate"))
		Activate(poolIndex,Vector2(-2682.0f, 200.0f),0.0f);
	ImGui::End();

	for (vector<Bullet*> i : pools)
		for (Bullet* j : i)
			j->Update();
}

void ProjectileManager::Render()
{
	for (vector<Bullet*> i : pools)
		for (Bullet* j : i)
			j->Render();
}

void ProjectileManager::Add(UINT poolSize)
{
	size.push_back(poolSize);
	pivots.push_back(0);
	pools.push_back(vector<Bullet*>());
}

void ProjectileManager::AddBullet(UINT poolIndex, Bullet* input)
{
	pools[poolIndex].push_back(input);	
}

void ProjectileManager::Activate(UINT poolIdx, Vector2 position, float z,Vector3 rotation)
{
	pools[poolIdx][pivots[poolIdx]]->SetPosition(position);
	pools[poolIdx][pivots[poolIdx]]->SetRotation(rotation);
	pools[poolIdx][pivots[poolIdx]]->Activate(z);
	while (1)
	{
		++pivots[poolIdx];
		pivots[poolIdx] %= size[poolIdx];
		if (pools[poolIdx][pivots[poolIdx]]->IsActivated()==0)break;
	}
}

bool Bullet::Intersect(IDamage * objects)
{
	return false;
}
