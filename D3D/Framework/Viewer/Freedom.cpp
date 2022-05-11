#include "Framework.h"
#include "Camera.h"
#include "Freedom.h"

Freedom::Freedom()
{
}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	ImGui::Begin("Camera");
	ImGui::SliderFloat("CameraSpeed",&moveSpeed ,200.0f ,2000.0f);
	ImGui::End();
	//Move
	if(move)
	{
		Vector3 position;
		Position(&position);
		if (!Keyboard::Get()->Press(VK_SHIFT))
			return;
		if (Keyboard::Get()->Press('D'))
			position += Vector3(1,0,0) * moveSpeed * Time::Delta();
		else if (Keyboard::Get()->Press('A'))
			position -= Vector3(1, 0, 0) * moveSpeed * Time::Delta();

		if (Keyboard::Get()->Press('W'))
			position += Vector3(0, 1, 0) * moveSpeed * Time::Delta();
		else if (Keyboard::Get()->Press('S'))
			position -= Vector3(0, 1, 0) * moveSpeed * Time::Delta();

		if (Keyboard::Get()->Down('Q'))
			position += Vector3(0, 0, 1);
		else if (Keyboard::Get()->Down('E'))
			position -= Vector3(0, 0, 1);
		Position(position);
	}
	
}

void Freedom::Speed(float moveSpeed, float rotationSpeed)
{
	this->moveSpeed = moveSpeed;
	this->rotationSpeed = rotationSpeed;
}

void Freedom::SetPosition(Vector3 position)
{
	if (!move)return;
	if (position.x < left) position.x = left;
	if (right < position.x)	position.x = right;
	Position(position);
}

void Freedom::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3(x, y, z));
}

void Freedom::SetLeft(float left)
{
	this->left = left;
}

void Freedom::SetRight(float right)
{
	this->right = right;
}
