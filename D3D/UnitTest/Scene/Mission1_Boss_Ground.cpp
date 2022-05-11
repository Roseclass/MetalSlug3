#include "stdafx.h"
#include "Mission1_Boss_Ground.h"
#include "LineEdit/LineEdit.h"
#include "Objects/ColliderManager.h"

Boss_Ground::Boss_Ground(UINT index, Vector2 scale, Vector2 position, ColliderManager* colliderManager, deque<Boss_Ground*>* pool)
	:colliderManager(colliderManager), scale(scale), pool(pool)
{
	wstring str = to_wstring(index);
	if (index < 10) str = L"0" + str;
	normal = new Sprite(L"background/Mission_1/BossStage/Mission_1_BossGround_M" + str + L".png", scale);
	head = new Sprite(L"background/Mission_1/BossStage/Mission_1_BossGround_L" + str + L".png", scale);
	effect = new Animation(L"background/Mission_1/BossStage/Mission_1_BossGround_Crash.png");
	effect->ReadFrameDatas(0); effect->SetScale(scale);
	effect->Stop();
	headOffset = (normal->GetTextureSize() - head->GetTextureSize());
	teamNumb = 0;
	health_Current = 1.0f;
	health_Max = 1.0f;
	//히트판정
	hitColldier = new Collider(Vector2(0.0f, 0.0f),Vector2(32.0f,75.0f),scale);
	line = new Line(2);
	MakeLine(index);
	colliderManager->AddGrounds(line);
	colliderManager->AddHitObjects((IDamage*)this);
	SetPosition(position);
	AddPool();
	limit = Context::Get()->GetViewport()->GetWidth()*0.5f + head->GetTextureSize().x*2.0f;
}

Boss_Ground::~Boss_Ground()
{
	SafeDelete(normal);
	SafeDelete(head);
	SafeDelete(hitColldier);
}

void Boss_Ground::Update()
{
	//static float x1 = -32.0f, y1 = 0.0f;
	//static float x2 = 32.0f, y2 = 1.0f;
	//ImGui::Begin("Ground");
	//{
		//if (ImGui::Button("change"))SetHead();
		//ImGui::SliderFloat("startX", &x1, -50.0f, 50.0f);
		//ImGui::SliderFloat("startY", &y1, -50.0f, 50.0f);
		//ImGui::SliderFloat("EndX", &x2, -50.0f, 50.0f);
		//ImGui::SliderFloat("EndY", &y2, -50.0f, 50.0f);
		//vertexLeftOffset = Vector2(x1, y1);
		//vertexRightOffset = Vector2(x2, y1+16.0f);
		//SetPosition(position);
	//}ImGui::End();
	normal->Update();
	head->Update();
	effect->Update();
	hitColldier->Update();
	line->Update();
}

void Boss_Ground::Move()
{
	position.x -= Time::Delta()*100.0f;
	SetPosition(position);
}

void Boss_Ground::Render()
{
	if (position.x > limit)return;
	line->Render();
	hitColldier->Render();
	if (isHead)head->Render();
	else normal->Render();
}

void Boss_Ground::TakeDamage(float damage)
{
	if (!isHead) return;
	GetBack();
}

bool Boss_Ground::CheckDead()
{
	if (health_Current <= 0.0f)return 1;
	return false;
}

Collider * Boss_Ground::GetHitCollider()
{
	return hitColldier;
}

void Boss_Ground::SetPosition(Vector3 position)
{
	SetPosition(position.x, position.y);
}

void Boss_Ground::SetPosition(Vector2 position)
{
	SetPosition(position.x, position.y);
}

void Boss_Ground::SetPosition(float x, float y)
{
	position = Vector2(x, y);
	normal->SetPosition(x, y);
	head->SetPosition(x + headOffset.x*scale.x, y + headOffset.y*scale.y);
	hitColldier->SetPosition(x,y);
	line->SetVertexPosition(0, Vector3(x + vertexLeftOffset.x*scale.x, y + vertexLeftOffset.y*scale.y, 0.0f));
	line->SetVertexPosition(1, Vector3(x + vertexRightOffset.x*scale.x, y + vertexRightOffset.y*scale.y, 0.0f));
}

void Boss_Ground::RenderEffect()
{
	effect->Render();
}

void Boss_Ground::AddPool()
{
	if (pool->size())
	{
		Vector2 pos = pool->back()->GetPosition();
		Vector2 size = normal->GetTextureSize();
		pos.x += size.x*scale.x*2.0f;
		SetPosition(pos);
	}else SetHead();
	health_Current = 1.0f;
	pool->push_back(this);
}
void Boss_Ground::GetBack()
{
	pool->pop_front();
	pool->front()->SetHead();
	isHead = 0;
	AddPool();
}

void Boss_Ground::MakeLine(UINT index)
{
	vertexLeftOffset.x=-32.0f;
	switch (index)
	{
	case 1:vertexLeftOffset.y=-18.0f;break;
	case 2:vertexLeftOffset.y=-2.0f;break;
	case 3:vertexLeftOffset.y=14.0f;break;
	case 4:vertexLeftOffset.y=30.0f;break;
	case 5:vertexLeftOffset.y=46.0f;break;
	case 6:vertexLeftOffset.y=30.0f;break;
	case 7:vertexLeftOffset.y=14.0f;break;
	case 8:vertexLeftOffset.y=-2.0f;break;
	case 9:vertexLeftOffset.y=-18.0f;break;
	case 10:vertexLeftOffset.y = 14.0f; break;
	case 11:vertexLeftOffset.y = 46.0f; break;
	default:break;
	}
	if(1<=index&&index<=4)vertexRightOffset= vertexLeftOffset+Vector2(64.0f,16.0f);
	else if(5 <= index && index <= 8)vertexRightOffset = vertexLeftOffset + Vector2(64.0f, -16.0f);
	else vertexRightOffset = vertexLeftOffset + Vector2(64.0f, 0.0f);
	hitColldier->SetOffset(0.0f, vertexLeftOffset.y - 75.0f);
}
