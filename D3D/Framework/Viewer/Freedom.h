#pragma once

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

	void Update() override;

	void Speed(float moveSpeed = 20.0f, float rotationSpeed = 2.0f);
	void SetPosition(Vector3 position);
	void SetPosition(float x, float y, float z);
	void SetLeft(float left);
	void SetRight(float right);
	void SetStop() { move = 0; }
	void SetMove() { move = 1; }

private:
	float moveSpeed = 200.0f;
	float rotationSpeed = 2.0f;

	float left=-10000.0f;
	float right = 0.0f;

	bool move = 1;
};