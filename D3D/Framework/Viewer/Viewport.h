#pragma once

class Viewport
{
public:
	Viewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	~Viewport();

	void RSSetViewport();
	void Set(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);

	float GetWidth() { return width; }
	float GetHeight() { return height; }

	//3D(WVP) -> 2D(SS)
	Vector3 Project(Vector3& source, Matrix& W, Matrix& V, Matrix& P);

	//2D(SS) -> 3D(WVP)
	Vector3 Unproject(Vector3& source, Matrix& W, Matrix& V, Matrix& P);

	void GetMousePos(Vector3 * pOutPosition);

private:
	float x, y;
	float width, height;
	float minDepth, maxDepth;

	D3D11_VIEWPORT viewport;
};