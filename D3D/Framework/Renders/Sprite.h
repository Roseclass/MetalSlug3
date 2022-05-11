#pragma once

class Sprite
{
public:
	Sprite(wstring spriteFileName,Vector2 scale=Vector2(1.0f,1.0f), Vector2 start = Vector2(0, 0), Vector2 end = Vector2(1.0f, 1.0f));
	Sprite(Texture* diffuseMap,Vector2 scale=Vector2(1.0f,1.0f), Vector2 start = Vector2(0, 0), Vector2 end = Vector2(1.0f, 1.0f));
	~Sprite();
	void Update();
	void Render();
	void SetTexture(wstring spriteFileName);
	void SetTexture(Texture* diffuseMap);
	void SetUv(Vector2 start = Vector2(0, 0), Vector2 end = Vector2(1, 1));
	void SetUv(float x1,float y1,float x2,float y2);
	void SetScale();
	void SetScale(Vector2 inScale);
	void SetScale(float x, float y);
	void SetRotation(Vector3 Rotation);
	void SetRotation(float x, float y,float z);
	void SetRotationDegree(Vector3 Rotation);
	void SetRotationDegree(float x, float y,float z);
	void SetPosition(Vector2 Position);
	void SetPosition(float x, float y);
	void SetLerpTime(float lerpTime);
	void DrawDebugLine(bool inBool);
	void Pass(UINT pass) { this->pass=pass; }

	Vector2 GetScale() { return scale; };
	wstring GetSpriteFileName() { return spriteFileName; };
	Shader* GetShader() { return shader; };
	Texture* GetTexture() { return diffuseMap; };
	Vector2 GetTextureSize() { return diffuseMap->GetTextureSize(); }
	PerFrame* GetPerFrame() { return perFrame; };
	Transform* GetTransform() { return transform; };
private:
	struct Desc
	{
		Vector3 Position;
		Vector2 Uv;
	}vertices[6];
	Shader* shader;
	VertexBuffer* buffer;
	UINT pass=0;

	Texture* diffuseMap;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;
	wstring spriteFileName = L"";

	Line* debugLine;
	bool drawLine=0;
	float lerpTime = 0.0f;

	PerFrame* perFrame;

	Transform* transform;

	Vector2 start, end, scale;
	void MakeVertices();
	void MakeUv();
	void MakeLine();
	void UpdateLine();
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Animation
{
public:
	Animation();
	Animation(wstring sheetFileName);
	~Animation();
	struct FrameData;
	void Update();
	void Render();
	void SetTexture(wstring spriteFileName);
	void SetTexture(Texture* diffuseMap);
	void SetUv(Vector2 start = Vector2(0, 0), Vector2 end = Vector2(1, 1), Vector2 offset = Vector2(0, 0));
	void SetUv(float x1, float y1, float x2, float y2,float offsetX=0.0f, float offsetY = 0.0f);
	void SetScale(Vector2 inScale);
	void SetScale(float x, float y);
	void SetRotation(Vector3 Rotation);
	void SetRotation(float x, float y, float z);
	void SetRotationDegree(Vector3 Rotation);
	void SetRotationDegree(float x, float y, float z);
	void SetPosition();
	void SetPosition(Vector2 Position);
	void SetPosition(float x, float y);
	void SetPerSecond(float perSecond);
	void SetSheetOffset(UINT sheetNumber, float x, float y);
	void SetStartNotify(UINT sheetNumber, UINT frameNumber,function<void()> notify);
	void SetEndNotify(UINT sheetNumber, UINT frameNumber,function<void()> notify);
	void SetLerpTime(float lerpTime);
	void DrawDebugLine(bool inBool);
	void AddSprite(wstring spriteFileName);
	void AddSprite(Texture* diffuseMap);
	void AddFrameData(UINT sheetNumber, float sx, float sy, float ex, float ey, float offsetX=0.0f, float offsetY = 0.0f);
	void AddFrameData(Vector2 start, Vector2 end, Vector2 offset = Vector2(0, 0));
	void AddSheetOffset(float x=0.0f, float y = 0.0f);
	void AddSheetOffset(Vector2 offset);
	void ChangeSheetNumber(UINT InSheetNumber);
	void Pass(UINT pass);
	void Play(UINT sheetNumber = 0);
	void Stop();
	void PlayLoop(UINT sheetNumber = 0);
	void PlayOnce(UINT sheetNumber = 0);
	void PlayOnceAndDisappear(UINT sheetNumber = 0);
	void PlayMontage(UINT sheetNumber);
	void SetLoop();
	void SetOnce();
	void SetOnceAndDisappear();
	void SetMontage();
	void EndMontage();
	void SetFrameNumb(UINT frameNumber);
	void ReadFrameDatas(UINT sheetNumber);
	void ExcuteStartNotify(UINT sheetNumber, UINT frameNumber);
	void ExcuteEndNotify(UINT sheetNumber, UINT frameNumber);

	bool IsPlay() { return sheetNumber>=0; }
	bool IsPlay(UINT numb) { return sheetNumber== numb; }
	bool IsMontagePlay() { return montage; }

	Texture* GetTexture( UINT sheetNumber) { return images[sheetNumber]->GetTexture(); };
	Texture* GetTexture() { return images[sheetNumber]->GetTexture(); };
	Vector3 GetPosition() { return originalPos; }
	Vector2 GetTextureSize(UINT sheetNumber) { return images[sheetNumber]->GetTexture()->GetTextureSize(); }
	Vector2 GetTextureSize() { return images[sheetNumber]->GetTexture()->GetTextureSize(); }
	PerFrame* GetPerFrame(UINT sheetNumber) { return images[sheetNumber]->GetPerFrame(); };
	PerFrame* GetPerFrame() { return images[sheetNumber]->GetPerFrame(); };
	Transform* GetTransform(UINT sheetNumber) { return images[sheetNumber]->GetTransform(); };
	Transform* GetTransform() { return images[sheetNumber]->GetTransform(); };
	Sprite* GetSheet(UINT numb) { return images[numb]; };
	UINT GetSheetSize() { return images.size(); };
	vector<FrameData*>& GetFrameDatas(UINT sheetNumber) { return datas[sheetNumber]; };
	vector<FrameData*>& GetFrameDatas() { return datas[sheetNumber]; };
private:
	struct FrameData
	{
		Vector2 start;
		Vector2 end;
		Vector2 offset;
		void SetStart(float x, float y)
		{
			start.x = x; start.y = y;
		}
		void SetStart(Vector2 vec)
		{
			SetStart(vec.x, vec.y);
		}
		void SetEnd(float x, float y)
		{
			end.x = x; end.y = y;
		}
		void SetEnd(Vector2 vec)
		{
			SetEnd(vec.x, vec.y);
		}
		void SetOffset(float x, float y)
		{
			offset.x = x; offset.y = y;
		}
		void SetOffset(Vector2 vec)
		{
			SetOffset(vec.x, vec.y);
		}
		FrameData(float sx, float sy, float ex, float ey,float x=0.0f,float y = 0.0f)
		{
			SetStart(sx, sy);
			SetEnd(ex, ey);
			SetOffset(x, y);
		}
		FrameData(Vector2 start, Vector2 end,Vector2 offset=Vector2(0,0))
		{
			SetStart(start);
			SetEnd(end);
			SetOffset(offset);
		}
		~FrameData() {};
		function<void()> notify_Start=nullptr;
		function<void()> notify_End=nullptr;
	};
	Vector3 originalPos=Vector3(0,0,0);
	vector<FrameData*> datas[100];
	vector<Sprite*> images;
	vector<Vector2> sheetOffset;
	int sheetNumber = 0;
	UINT frameNumber = 0;
	float lerpTime;
	float perSecond=10.0f;
	float prevTime = 0.0f;
	//업데이트 판단
	UINT prevSheet=0;
	UINT prevFrame=0;
	//몽타주
	struct Montage
	{
		bool loop = 0;
		bool disappear = 0;
		int sheet= 0;
		UINT frame= 0;
		float playTime= 0;
	}prev;
	bool line = 0;
	bool loop = true;
	bool disappear = false;
	bool montage = false;
};
