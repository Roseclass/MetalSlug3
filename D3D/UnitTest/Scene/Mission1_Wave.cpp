#include "stdafx.h"
#include "Mission1_Wave.h"
#include "Objects/ColliderManager.h"
#include "Interfaces/ICharacter.h"

Wave::Wave(float x2, float y2, wstring directory, Vector2 scale, ColliderManager* colliderManager)
	:colliderManager(colliderManager)
{
	wave_Lower = new Animation(directory + L"Mission_1_Tide_Lower.png");
	wave_Lower->SetScale(scale); wave_Lower->ReadFrameDatas(0);
	wave_Lower->SetPosition(
		wave_Lower->GetTextureSize(0).x*scale.x - x2,
		40.0f*scale.y - y2 + 32.0f * scale.y);
	wave_Upper = new Animation(directory + L"Mission_1_Tide_Upper.png");
	wave_Upper->SetScale(scale); wave_Upper->ReadFrameDatas(0);
	wave_Upper->SetPosition(
		wave_Upper->GetTextureSize(0).x*scale.x - x2,
		104.0f*scale.y - y2 + 32.0f * scale.y);
	splash = new Animation(L"character/effect/Splash_Water.png");
	splash->SetScale(scale*0.2f); splash->ReadFrameDatas(0);
	splash->SetPerSecond(20.0f);splash->SetSheetOffset(0,0.0f,25.0f);

	left = -x2; right = left + (wave_Lower->GetTextureSize(0).x - 10.0f)*scale.x*2.0f;
	top = 80.0f*scale.y - y2 + 32.0f * scale.y;
}

Wave::~Wave()
{
	SafeDelete(splash);
	SafeDelete(wave_Upper);
	SafeDelete(wave_Lower);
}

void Wave::Update()
{
	splash->Update();
	wave_Upper->Update();
	wave_Lower->Update();
}

void Wave::RenderCover()
{
	if (target == nullptr)return;
	if (left<=target->GetPosition().x&&
		target->GetPosition().x<=right&&
		target->GetPosition().y<=top)
	{
		splash->SetPosition(target->GetPosition().x, top);
		splash->Render();
	}
	wave_Lower->Render();
}

void Wave::RenderBackground()
{
	wave_Upper->Render();
}
void Wave::SetPlayerTarget(ICharacter * target)
{
	colliderManager->FindPlayerPosition(&this->target);
}
