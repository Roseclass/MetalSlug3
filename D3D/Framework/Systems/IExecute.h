#pragma once

class IExecute
{
public:
	virtual void Initialize() = 0;
	virtual void Destroy() = 0;

	virtual void Update() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

	virtual void ResizeScreen() = 0;

	virtual void SetMain(IExecute* main) = 0;
	virtual void Start() = 0;
};