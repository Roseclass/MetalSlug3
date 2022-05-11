#pragma once

class PerFrame
{
public:
	PerFrame(Shader* shader);
	~PerFrame();

	void Update();
	void Render();
	float GetTime() { return desc.Time; };
private:


	struct Desc
	{
		Matrix View;
		Matrix Projection;
		Matrix VP;

		float Time;
		float Padding[3];
	}desc;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
};