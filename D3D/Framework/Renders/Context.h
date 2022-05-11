#pragma once

class Context
{
public:
	static Context* Get();
	static void Create();
	static void Delete();

private:
	Context();
	~Context();

public:
	void ResizeScreen();

	void Update();
	void Render();

	D3DXMATRIX View();
	D3DXMATRIX Projection();

	Orthographic* GetPerspective() { return perspective; }
	Viewport* GetViewport() { return viewport; }
	Camera* GetCamera() { return camera; }

private:
	static Context* instance;

private:
	Orthographic* perspective;
	Viewport* viewport;

	Camera* camera;
};