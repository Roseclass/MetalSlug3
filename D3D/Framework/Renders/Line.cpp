#include "Framework.h"
#include "Line.h"

Line::Line()
{
	shader = new Shader(L"02_LineColor.fxo");

	vertices = new Desc[MAX_DEBUG_LINE];
	//ZeroMemory(vertices, sizeof(Desc) * MAX_DEBUG_LINE);
	stride = sizeof(Desc);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = stride *MAX_DEBUG_LINE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer));

	transform = new Transform(shader);
	perFrame = new PerFrame(shader);
}

Line::Line(UINT count):count(count)
{
	shader = new Shader(L"02_LineColor.fxo");

	vertices = new Desc[2];
	//ZeroMemory(vertices, sizeof(Desc) * MAX_DEBUG_LINE);
	stride = sizeof(Desc);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = stride * MAX_DEBUG_LINE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer));

	transform = new Transform(shader);
	perFrame = new PerFrame(shader);
}

Line::~Line()
{
	SafeDelete(perFrame);
	SafeDelete(transform);
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void Line::Update()
{
	transform->Render();
	perFrame->Update();
	perFrame->Render();
}

void Line::Render()
{
	D3D::GetDC()->UpdateSubresource(vertexBuffer, 0, nullptr, vertices, sizeof(Desc) * count, 0);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	shader->Draw(0, 0, count);
}

void Line::SetVertexPosition(UINT idx, Vector2 position)
{
	vertices[idx].position.x = position.x;
	vertices[idx].position.y = position.y;
}

void Line::SetVertexPosition(UINT idx, Vector3 position)
{
	vertices[idx].position.x = position.x;
	vertices[idx].position.y = position.y;
}

void Line::SetVertexColor(Color color)
{
	for (UINT i = 0; i < count; i++)
		vertices[i].color = color;
}

void Line::SetVertexColor(UINT idx, Color color)
{
	vertices[idx].color = color;
}

void Line::AddVertex()
{
	if (count++==0)
	{
		Vector3 pos;
		Context::Get()->GetCamera()->Position(&pos);
		SetVertexPosition(0, pos);
		return;
	}
	float x = vertices[count-2].position.x + 10.0f;
	float y = vertices[count-2].position.y + 10.0f;
	vertices[count - 1].position.x = x;
	vertices[count - 1].position.y = y;
}

void Line::AddVertex(Vector2 position)
{
	AddVertex(Vector3(position.x, position.y, 0.0f));
}

void Line::AddVertex(Vector3 position)
{
	if (count++==0)SetVertexPosition(0, position);
	else SetVertexPosition(count-1, position);
}

void Line::DeleteVertex(int idx)
{
	if (!count)return;
	if (idx == -1)idx += count;
	for (UINT i = idx; i < count - 1; i++)
	{
		vertices[i].position = vertices[i + 1].position;
		vertices[i].color = vertices[i + 1].color;
	}
	count--;
}

void Line::GetLine(Vector2 location, Vector2 * start, Vector2 * end)
{
	if (location.x < vertices[0].position.x || vertices[count-1].position.x < location.x) 
	{ 
		start->x = 123456789;
		return;
	}
	if (count < 2)
	{
		start->x = 123456789;
		return;
	}
	float x = location.x;
	int s = 0, e = count-1, mid = (s + e) / 2;
	while (s < e)
	{
		if (e - s == 1)break;
		if (vertices[mid].position.x < x)s = mid;		
		else e = mid;		
		mid = (s + e) / 2;
	}
	start->x = vertices[s].position.x; end->x = vertices[e].position.x;
	start->y = vertices[s].position.y; end->y = vertices[e].position.y;
}

bool Line::GetVertexPosition(UINT idx, Vector2* pPosition)
{
	if (idx >= count) return 0;
	float x = vertices[idx].position.x;
	float y = vertices[idx].position.y;
	*pPosition = Vector2(x, y);
	return 1;
}

Collider::Collider(Vector2 position, Vector2 size,Vector2 scale)
{
	shader = new Shader(L"02_LineColor.fx");
	MakeVertices();
	this->size = size;
	count = 5;
	stride = sizeof(Desc);
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = count * stride;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer));

	transform = new Transform(shader);
	perFrame = new PerFrame(shader);

	SetPosition(position);
	SetScale(scale);
}

Collider::~Collider()
{
	SafeDelete(perFrame);
	SafeDelete(transform);
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void Collider::Update()
{
	transform->Render();
	perFrame->Update();
	perFrame->Render();
}

void Collider::Render()
{
	if (collision) SetLineColor(Color(1, 0, 0, 1));
	else SetLineColor(Color(0, 1, 0, 1));
	D3D::GetDC()->UpdateSubresource(vertexBuffer, 0, nullptr, vertices, sizeof(Desc) * count, 0);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	shader->Draw(0, 0, count);
}

void Collider::SetScale(float x, float y)
{
	scale.x = x; scale.y = y;
	transform->Scale(scale.x*size.x, scale.y*size.y,1.0f);
}

void Collider::SetScale(Vector2 scale)
{
	SetScale(scale.x, scale.y);
}

void Collider::SetScale(Vector3 scale)
{
	SetScale(scale.x, scale.y);
}

void Collider::SetRotationDegree(float x, float y, float z)
{
	transform->RotationDegree(x, y,z);
}

void Collider::SetRotationDegree(Vector3 rotation)
{
	SetRotationDegree(rotation.x, rotation.y, rotation.z);
}

void Collider::SetPosition()
{
	transform->Position(position.x+ scale.x*offset.x, position.y+ scale.y*offset.y, 0.0f);
}

void Collider::SetPosition(float x, float y)
{
	position.x = x;	position.y = y;
	x += scale.x*offset.x;
	y += scale.y*offset.y;
	transform->Position(x, y, 0.0f);
}

void Collider::SetPosition(Vector2 position)
{
	SetPosition(position.x, position.y);
}

void Collider::SetPosition(Vector3 position)
{
	SetPosition(position.x, position.y);
}

void Collider::SetOffset(float x, float y)
{
	offset = Vector2(x, y);
	SetPosition();
}

void Collider::SetOffset(Vector2 position)
{
	SetOffset(position.x, position.y);
}

void Collider::SetSize(float x, float y)
{
	size = Vector2(x, y);
	SetScale(scale);
}

void Collider::SetSize(Vector2 position)
{
	SetSize(position.x, position.y);
}

void Collider::SetLineColor(Color color)
{
	for (int i = 0; i < 5; i++)
		vertices[i].color = color;
}

void Collider::StartWorldCollsionCheck()
{
	collision = 0;
}

void Collider::ReverseOffsetX()
{
	offset.x *= -1.0f;
}

void Collider::ReverseOffsetY()
{
	offset.y *= -1.0f;
}

float Collider::GetLeft()
{
	return (position.x + (offset.x - size.x)*scale.x);
}

float Collider::GetRight()
{
	return (position.x + (offset.x + size.x)*scale.x);
}

float Collider::GetTop()
{
	return (position.y + (offset.y + size.y)*scale.y);
}

float Collider::GetBottom()
{
	return (position.y + (offset.y - size.y)*scale.y);
}

bool Collider::Intersect(float x, float y)
{
	Vector3 temp;
	transform->Position(&temp);
	if (x < temp.x - size.x*scale.x || temp.x + size.x*scale.x < x)return 0;
	if (y < temp.y - size.y*scale.y || temp.y + size.y*scale.y < y)return 0;
	//collision = 1;
	return true;
}

bool Collider::Intersect(Vector2 pos)
{
	return Intersect(pos.x,pos.y);
}

bool Collider::Intersect(Collider * collider)
{
	return false;
}

void Collider::MakeVertices()
{
	vertices = new Desc[5];
	vertices[0].position = Vector3(-1.0f, +1.0f, 0.0f);	//좌상
	vertices[1].position = Vector3(-1.0f, -1.0f, 0.0f); //좌하
	vertices[2].position = Vector3(+1.0f, -1.0f, 0.0f); //우하
	vertices[3].position = Vector3(+1.0f, +1.0f, 0.0f); //우상
	vertices[4].position = Vector3(-1.0f, +1.0f, 0.0f); //좌상
}

ConvexHull::ConvexHull(Vector2 position, const vector<Vector3>& vertices, Vector2 scale)
{
	shader = new Shader(L"02_LineColor.fx");
	positions = vertices;
	size = Vector2(1.0f, 1.0f);
	MakeVertices();
	count = vertices.size()+1;
	stride = sizeof(Desc);
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = count * stride;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = this->vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer));

	transform = new Transform(shader);
	perFrame = new PerFrame(shader);

	SetPosition(position);
	SetScale(scale);
}

ConvexHull::~ConvexHull()
{

}

void ConvexHull::MakeVertices()
{
	vertices = new Desc[positions.size()+1];
	for (int i = 0; i < positions.size(); i++)
		vertices[i].position = positions[i];
	vertices[positions.size()].position = positions[0];
}
