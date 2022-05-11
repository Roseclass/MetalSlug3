#include "Framework.h"
#include "Camera.h"

Camera::Camera()
{
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matView);

	Rotation();
	Move();
}

Camera::~Camera()
{
}

void Camera::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void Camera::Position(Vector3 & position)
{
	this->position = position;

	Move();
}

void Camera::Position(Vector3 * position)
{
	*position = this->position;
}

void Camera::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Camera::Rotation(Vector3 & rotation)
{
	this->rotation = rotation;

	Rotation();
}


void Camera::Rotation(Vector3 * rotation)
{
	*rotation = this->rotation;
}

void Camera::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Camera::RotationDegree(Vector3 & rotation)
{
	this->rotation = rotation * 0.017453f;

	Rotation(this->rotation);
}

void Camera::RotationDegree(Vector3 * rotation)
{
	*rotation = this->rotation * 57.295791f;
}

void Camera::GetMatrix(Matrix * matrix)
{
	memcpy(matrix, &matView, sizeof(Matrix));
}

void Camera::Rotation()
{
	
}

void Camera::Move()
{
	View();
}

void Camera::View()
{
	D3DXMatrixLookAtLH(&matView, &position, &(position + at), &up);
}
