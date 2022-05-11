#pragma once

#define MAX_DEBUG_LINE 100
class Line
{
public:
	Line();
	Line(UINT count);
	~Line();
	void Update();
	void Render();
	void SetVertexPosition(UINT idx, Vector2 position);
	void SetVertexPosition(UINT idx, Vector3 position);
	void SetVertexColor(Color color);
	void SetVertexColor(UINT idx, Color color);
	void AddVertex();
	void AddVertex(Vector2 position);
	void AddVertex(Vector3 position);
	void DeleteVertex(int idx = - 1);
	void GetLine(Vector2 location, Vector2* start, Vector2* end);
	bool GetVertexPosition(UINT idx,Vector2* pPosition);

	UINT Count() { return count; };
	UINT Stride() { return stride; };
private:
	struct Desc
	{
		Vector3 position;
		Color color;
		Desc() : position(0, 0, 0), color(0, 1, 0, 1) {}
		Desc(float x,float y, float r=1.0f, float g = 0.0f, float b = 0.0f)
		{
			position.x = x;	position.y = y; position.z = 0.0f;
			color.r = r;color.g = g;color.b = b;
		}
	};
	Shader* shader;
	PerFrame* perFrame;
	Transform* transform;
	ID3D11Buffer* vertexBuffer;
	Desc* vertices;

	UINT stride;
	UINT count=0;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Collider
{
public:
	Collider() {};
	Collider(Vector2 position, Vector2 size,Vector2 scale= Vector2(1.0f,1.0f));
	~Collider();

	void Update();
	void Render();
	void SetScale(float x, float y);
	void SetScale(Vector2 scale);
	void SetScale(Vector3 scale);
	void SetRotationDegree(float x, float y,float z);
	void SetRotationDegree(Vector3 rotation);
	void SetPosition();
	void SetPosition(float x, float y);
	void SetPosition(Vector2 position);
	void SetPosition(Vector3 position);
	void SetOffset(float x, float y);
	void SetOffset(Vector2 position);
	void SetSize(float x, float y);
	void SetSize(Vector2 position);
	void SetLineColor(Color color);
	void StartWorldCollsionCheck();
	void ReverseOffsetX();
	void ReverseOffsetY();

	Transform* GetTransform() { return transform; };
	Vector2 GetScale() { return scale; }
	Vector2 GetSize() { return Vector2(scale.x*size.x, scale.y*size.y); }
	Vector2 GetOffset() { return offset; }

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	bool Intersect(float x, float y);
	bool Intersect(Vector2 pos);
	bool Intersect(Collider* collider);

	UINT Count() { return stride; };
	UINT Stride() { return count; };
protected:
	struct Desc
	{
		Vector3 position;
		Color color;
		Desc() : position(0, 0, 0), color(0, 1, 0, 1) {}
		Desc(float x, float y, float r = 1.0f, float g = 0.0f, float b = 0.0f)
		{
			position.x = x;	position.y = y; position.z = 0.0f;
			color.r = r; color.g = g; color.b = b;
		}
	};
	Shader* shader;
	PerFrame* perFrame;
	Transform* transform;
	ID3D11Buffer* vertexBuffer;
	Desc* vertices;

	Vector2 position;
	Vector2 size;
	Vector2 offset=Vector2(0,0);
	Vector2 scale=Vector2(1.0f,1.0f);

	UINT stride;
	UINT count = 0;

	bool collision = 0;

	virtual void MakeVertices();
};

class ConvexHull : public Collider
{
public:
	ConvexHull(Vector2 position,const vector<Vector3>& vertices, Vector2 scale = Vector2(1.0f, 1.0f));
	~ConvexHull();
	vector<Vector3>* GetVertices() { return &positions;};
private:
	virtual void MakeVertices() override;
	vector<Vector3> positions;
};