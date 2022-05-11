#include "Framework.h"
#include "Context.h"

Context* Context::instance = NULL;

Context * Context::Get()
{
	//assert(instance != NULL);

	return instance;
}

void Context::Create()
{
	assert(instance == NULL);

	instance = new Context();
}

void Context::Delete()
{
	SafeDelete(instance);
}

Context::Context()
{
	D3DDesc desc = D3D::GetDesc();

	perspective = new Orthographic(desc.Width, desc.Height,-1.0f,1.0f);
	viewport = new Viewport(desc.Width, desc.Height,0.0f,0.0f);

	camera = new Freedom();
}

Context::~Context()
{
	SafeDelete(perspective);
	SafeDelete(viewport);
}

void Context::ResizeScreen()
{
	perspective->Set(D3D::Width(), D3D::Height());
	viewport->Set(D3D::Width(), D3D::Height());
}

void Context::Update()
{
	camera->Update();
}

void Context::Render()
{
	viewport->RSSetViewport();

	string str = string("FPS : ") + to_string(ImGui::GetIO().Framerate);
	Gui::Get()->RenderText(5, 5, 1, 1, 1, str);

	Vector3 cameraPosition;
	camera->Position(&cameraPosition);

	Vector3 cameraRotation;
	camera->RotationDegree(&cameraRotation);

	str = "Camera(P) : ";
	str += to_string((int)cameraPosition.x) + ", " + to_string((int)cameraPosition.y) + ", " + to_string((int)cameraPosition.z);
	Gui::Get()->RenderText(5, 20, 1, 1, 1, str);

	str = "Camera(R) : ";
	str += to_string((int)cameraRotation.x) + ", " + to_string((int)cameraRotation.y);
	Gui::Get()->RenderText(5, 35, 1, 1, 1, str);
}

D3DXMATRIX Context::View()
{
	Matrix view;
	camera->GetMatrix(&view);

	return view;
}

D3DXMATRIX Context::Projection()
{
	D3DXMATRIX projection;
	//perspective->GetMatrix(&projection);
	float Width = Context::Get()->GetViewport()->GetWidth();
	float Height = Context::Get()->GetViewport()->GetHeight();
	D3DXMatrixOrthoOffCenterLH(&projection, -(float)(Width *0.5f),
		(float)(Width*0.5f),
		-(float)(Height*0.5f),
		(float)(Height*0.5f), -1, 1000);

	return projection;
}
