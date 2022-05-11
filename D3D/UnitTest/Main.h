#pragma once
#include "Systems/IExecute.h"

class Main : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override;
	virtual void SetMain(IExecute* main)override {};
	virtual void Start() override {};

	void Push(IExecute* execute);
	void NextScene();
	void PopFront();
	void LoadMission1();
	void LoadMission1_Boss();
private:
	vector<IExecute *> executes;
	UINT loadCount = 0;
	UINT sceneCount = 0;
	thread* t1;

};