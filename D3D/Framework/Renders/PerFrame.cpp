#include "Framework.h"
#include "PerFrame.h"

PerFrame::PerFrame(Shader* shader)
{
	buffer = new ConstantBuffer((void*)&desc, sizeof(Desc));
	sBuffer = shader->AsConstantBuffer("CB_PerFrame");
}

PerFrame::~PerFrame()
{
	SafeDelete(buffer);
	SafeRelease(sBuffer);
}

void PerFrame::Update()
{
	desc.Time = Time::Get()->Running();
	desc.View = Context::Get()->View();
	desc.Projection = Context::Get()->Projection();	
	desc.VP = desc.View*desc.Projection;
}

void PerFrame::Render()
{
	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());
}

