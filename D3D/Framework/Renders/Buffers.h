#pragma once

class VertexBuffer
{
public:
	VertexBuffer(void* data, UINT count, UINT stride, UINT slot = 0, bool bCputWrite = false, bool bGpuWrite = false);
	~VertexBuffer();
	UINT Count() { return count; }
	UINT Stride() { return stride; }

	ID3D11Buffer* Buffer() { return buffer; }
	void Render();
private:
	ID3D11Buffer* buffer;

	void* data;
	UINT count;
	UINT stride;
	UINT slot;

	bool bCputWrite;
	bool bGpuWrite;
};

class ConstantBuffer
{
public:
	ConstantBuffer(void* data, UINT dataSize);
	~ConstantBuffer();

	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;
};