#include "stdafx.h"
#include "Main.h"
#include "Systems/Window.h"
#include "Scene/SelectScene.h"
#include "Scene/TutorialScene.h"
#include "Scene/Mission1.h"
#include "Scene/Mission1_Boss.h"

void Main::Initialize()
{
	Push(new SelectScene());
}

void Main::Destroy()
{
	for (IExecute* exe : executes)
	{
		if (exe == nullptr) continue;
		exe->Destroy();
		SafeDelete(exe);
	}
	t1->join();
}

void Main::Update()
{
	executes[sceneCount]->Update();
	//for (IExecute* exe : executes)
	//	exe->Update();
}

void Main::PreRender()
{
	executes[sceneCount]->PreRender();
	//for (IExecute* exe : executes)
	//	exe->PreRender();
}

void Main::Render()
{
	executes[sceneCount]->Render();
	//for (IExecute* exe : executes)
	//	exe->Render();
}

void Main::PostRender()
{
	executes[sceneCount]->PostRender();
	//for (IExecute* exe : executes)
	//	exe->PostRender();
}

void Main::ResizeScreen()
{
	for (IExecute* exe : executes)
		exe->ResizeScreen();
}

void Main::Push(IExecute * execute)
{
	execute->SetMain((IExecute *)this);
	executes.push_back(execute);
	execute->Initialize();
}

void Main::PopFront()
{
	for (UINT i = 0; i < sceneCount; i++)
	{
		if (executes[i] == nullptr)continue;
		executes[i]->Destroy();
		SafeDelete(executes[i]);
	}
}

void Main::LoadMission1()
{
	t1 = new thread([this]() { Push(new Mission1()); loadCount++; });
}

void Main::LoadMission1_Boss()
{
	t1->join();
	t1 = new thread([this]() { PopFront(); Push(new Mission1_Boss()); loadCount++; });
}

void Main::NextScene()
{
	if (loadCount == sceneCount)return;
	sceneCount++;
	executes[sceneCount]->Start();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	D3DDesc desc;
	desc.AppName = L"MetalSlug3";
	desc.Instance = instance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Handle = NULL;
	desc.Width = 960;
	desc.Height = 720;
	desc.Background = Color(0.3f, 0.3f, 0.3f, 1.0f);
	D3D::SetDesc(desc);

	Main* main = new Main();
	WPARAM wParam = Window::Run(main);

	SafeDelete(main);

	return wParam;
}