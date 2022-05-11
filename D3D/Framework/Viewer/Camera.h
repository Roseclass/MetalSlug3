#pragma once
class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update() = 0;

public:
	void Position(float x, float y, float z);
	void Position(Vector3& position);
	void Position(Vector3* position);

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& rotation);
	void Rotation(Vector3* rotation);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& rotation);
	void RotationDegree(Vector3* rotation);

	void GetMatrix(Matrix* matrix);
protected:
	virtual void Rotation();
	virtual void Move();

protected:
	void View();

private:
	Vector3 position = Vector3(0.0f, 0.0f,-1.0f);
	Vector3 rotation = Vector3(0, 0, 0);

	Vector3 at = Vector3(0.0f, 0.0f, 1.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);


	Matrix matRotation;
	Matrix matView;
};