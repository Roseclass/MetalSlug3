#include "Framework.h"
#include "Sprite.h"
#include "Viewer/Camera.h"

Sprite::Sprite(wstring spriteFileName, Vector2 scale, Vector2 start, Vector2 end)
	: spriteFileName(spriteFileName)
	, start(start)
	, end(end)
	, scale(scale)
{
	shader = new Shader(L"04_Rect.fxo");

	MakeVertices();
	MakeUv();
	buffer = new VertexBuffer(vertices, 6, sizeof(Desc),0,0,1);

	perFrame = new PerFrame(shader);

	transform = new Transform(shader);
	debugLine = new Line();
	MakeLine();

	diffuseMap = new Texture(spriteFileName);
	sDiffuseMap = shader->AsSRV("DiffuseMap");
	SetTexture(spriteFileName);

	SetScale();
	SetRotation(0, 0, 0);
	SetPosition(0,0);
}

Sprite::Sprite(Texture * diffuseMap, Vector2 scale, Vector2 start, Vector2 end)
	: start(start)
	, end(end)
	, scale(scale)
{
	shader = new Shader(L"04_Rect.fxo");

	MakeVertices();
	MakeUv();
	buffer = new VertexBuffer(vertices, 6, sizeof(Desc), 0, 0, 1);

	perFrame = new PerFrame(shader);

	transform = new Transform(shader);
	debugLine = new Line();
	MakeLine();

	this->diffuseMap = diffuseMap;
	spriteFileName = diffuseMap->GetFile();
	sDiffuseMap = shader->AsSRV("DiffuseMap");
	SetTexture(spriteFileName);

	SetScale();
	SetRotation(0, 0, 0);
	SetPosition(0, 0);

}

Sprite::~Sprite()
{
	SafeDelete(buffer);
	SafeDelete(diffuseMap);
	SafeDelete(perFrame);
	SafeDelete(transform);
	SafeDelete(shader);
}

void Sprite::Update()
{	
	perFrame->Update();
	UpdateLine();
	debugLine->Update();
}

void Sprite::Render()
{
	if(drawLine)debugLine->Render();
	lerpTime -= Time::Delta();
	if (lerpTime < 0.0f) lerpTime = 0.0f;
	GetShader()->AsScalar("lerpTime")->SetFloat(lerpTime);
	transform->Render();
	perFrame->Render();
	buffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	shader->Draw(0, pass, 6);
}

void Sprite::SetTexture(wstring spriteFileName)
{
	SafeDelete(diffuseMap);
	this->spriteFileName = spriteFileName;
	diffuseMap = new Texture(spriteFileName);
	sDiffuseMap->SetResource(diffuseMap->SRV());
	SetScale();
}

void Sprite::SetTexture(Texture * diffuseMap)
{
	spriteFileName = diffuseMap->GetFile();
	this->diffuseMap = diffuseMap;
	sDiffuseMap->SetResource(diffuseMap->SRV());
	SetScale();
}

void Sprite::SetUv(Vector2 start, Vector2 end)
{
	if ((int)start.x >= 1 || (int)start.y >= 1||
		(int)end.x > 1 || (int)end.y > 1)
	{
		start.x /= diffuseMap->GetWidth();
		end.x /= diffuseMap->GetWidth();
		start.y /= diffuseMap->GetHeight();
		end.y /= diffuseMap->GetHeight();
	}

	this->start = start;this->end = end;
	SetScale();

	vertices[0].Uv = Vector2(start.x, end.y);
	vertices[1].Uv = Vector2(start.x, start.y);
	vertices[2].Uv = Vector2(end.x, end.y);
	vertices[3].Uv = Vector2(end.x, end.y);
	vertices[4].Uv = Vector2(start.x, start.y);
	vertices[5].Uv = Vector2(end.x, start.y);

	D3D::GetDC()->UpdateSubresource(buffer->Buffer(), 0, nullptr, vertices, buffer->Stride() * buffer->Count(), 0);
}

void Sprite::SetUv(float x1, float y1, float x2, float y2)
{
	SetUv(Vector2(x1, y1), Vector2(x2, y2));
}

void Sprite::SetScale()
{
	float x = scale.x*diffuseMap->GetWidth()*(end.x - start.x);
	float y = scale.y*diffuseMap->GetHeight()*(end.y - start.y);
	transform->Scale(x,y,1.0f);
}

void Sprite::SetScale(Vector2 inScale)
{
	scale = inScale;
	SetScale();
}

void Sprite::SetScale(float x, float y)
{
	scale.x = x;scale.y = y;
	SetScale();
}

void Sprite::SetRotation(Vector3 Rotation)
{
	transform->Rotation(Rotation.x, Rotation.y, Rotation.z);
}

void Sprite::SetRotation(float x, float y, float z)
{
	transform->Rotation(x, y, z);
}

void Sprite::SetRotationDegree(Vector3 Rotation)
{
	transform->RotationDegree(Rotation);
}

void Sprite::SetRotationDegree(float x, float y, float z)
{
	transform->RotationDegree(x, y, z);
}

void Sprite::SetPosition(Vector2 Position)
{
	transform->Position(Position.x, Position.y, 0);
}

void Sprite::SetPosition(float x, float y)
{
	transform->Position(x, y, 0);
}

void Sprite::SetLerpTime(float lerpTime)
{
	this->lerpTime = lerpTime;
}

void Sprite::DrawDebugLine(bool inBool)
{
	drawLine = inBool;
}

void Sprite::MakeVertices()
{
	//Position
	vertices[0].Position = Vector3(-1.0f, -1.0f, 0.0f);	vertices[3].Position = Vector3(+1.0f, -1.0f, 0.0f);
	vertices[1].Position = Vector3(-1.0f, +1.0f, 0.0f);	vertices[4].Position = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[2].Position = Vector3(+1.0f, -1.0f, 0.0f);	vertices[5].Position = Vector3(+1.0f, +1.0f, 0.0f);
}

void Sprite::MakeUv()
{
	//Uv
	vertices[0].Uv = Vector2(start.x, end.y);	vertices[3].Uv = Vector2(end.x, end.y);
	vertices[1].Uv = Vector2(start.x, start.y);	vertices[4].Uv = Vector2(start.x, start.y);
	vertices[2].Uv = Vector2(end.x, end.y);		vertices[5].Uv = Vector2(end.x, start.y);
}

void Sprite::MakeLine()
{
	Vector3 center, scale;
	transform->Position(&center);
	transform->Scale(&scale);
	debugLine->AddVertex(Math::mul(vertices[0].Position, scale) + center);
	debugLine->AddVertex(Math::mul(vertices[1].Position, scale) + center);
	debugLine->AddVertex(Math::mul(vertices[5].Position, scale) + center);
	debugLine->AddVertex(Math::mul(vertices[2].Position, scale) + center);
	debugLine->AddVertex(Math::mul(vertices[0].Position, scale) + center);
}

void Sprite::UpdateLine()
{
	Vector3 center, scale;
	transform->Position(&center);
	transform->Scale(&scale);
	debugLine->SetVertexPosition(0, Math::mul(vertices[0].Position, scale) + center);
	debugLine->SetVertexPosition(1, Math::mul(vertices[1].Position, scale) + center);
	debugLine->SetVertexPosition(2, Math::mul(vertices[5].Position, scale) + center);
	debugLine->SetVertexPosition(3, Math::mul(vertices[2].Position, scale) + center);
	debugLine->SetVertexPosition(4, Math::mul(vertices[0].Position, scale) + center);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Animation::Animation()
{
	prevTime = Time::Get()->Running();
}
Animation::Animation(wstring sheetFileName)
{
	AddSprite(sheetFileName);
	prevTime = Time::Get()->Running();
}

Animation::~Animation()
{
	for (UINT i = 0; i < images.size(); i++)
	{
		for(UINT k=0; k<datas[i].size();k++)
			SafeDelete(datas[i][k]);
		SafeDelete(images[i]);
	}
}

void Animation::Update()
{
	float curTime = Time::Get()->Running();
	if (sheetNumber == -1)//재생x
	{
		prevSheet = sheetNumber;
		frameNumber = 0;
		return;
	}
	if (datas[sheetNumber].empty())//프레임정보없음
	{
		SetPosition();
		images[sheetNumber]->Update();
		return;
	}
	if (prevSheet != sheetNumber)//애니메이션 바뀔때
	{
		prevSheet = sheetNumber;
		prevTime = curTime;
		frameNumber = 0;
		SetUv(datas[sheetNumber][frameNumber]->start, datas[sheetNumber][frameNumber]->end, datas[sheetNumber][frameNumber]->offset);
		SetPosition(originalPos.x,originalPos.y);
	}
	else if (curTime - prevTime >= 1.0f / perSecond)//프레임 넘어갈때
	{
		prevTime = curTime;
		prevFrame = frameNumber;
		frameNumber++;
		if (frameNumber >= datas[sheetNumber].size())
		{
			loop ? frameNumber = 0 : frameNumber = datas[sheetNumber].size()-1;
			if (!loop && disappear) sheetNumber = -1;
			if (montage)EndMontage();
		}
		if (sheetNumber < 0)return;
		SetUv(datas[sheetNumber][frameNumber]->start, datas[sheetNumber][frameNumber]->end, datas[sheetNumber][frameNumber]->offset);
		if (frameNumber != prevFrame)//한프레임이 계속재생되는경우 노티파이 호출 x
		{
			ExcuteEndNotify(prevSheet,prevFrame);
			ExcuteStartNotify(sheetNumber,frameNumber);
		}
	}
	SetPosition();
	images[sheetNumber]->Update();
}

void Animation::Render()
{
	if (sheetNumber < 0)return;
	//images[sheetNumber]->DrawDebugLine(line);
	if (images[sheetNumber])
	{
		images[sheetNumber]->SetLerpTime(lerpTime);
		images[sheetNumber]->Render();
		lerpTime -= Time::Delta();
		if (lerpTime < 0.0f) lerpTime = 0.0f;
	}
}

void Animation::SetTexture(wstring spriteFileName)
{
	if (!images[sheetNumber])return;
	images[sheetNumber]->SetTexture(spriteFileName);
}

void Animation::SetTexture(Texture * diffuseMap)
{
	if (!images[sheetNumber])return;
	images[sheetNumber]->SetTexture(diffuseMap);
}

void Animation::SetUv(Vector2 start, Vector2 end, Vector2 offset)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	images[sheetNumber]->SetUv(start, end);
	/*Vector3 temp;
	images[sheetNumber]->GetTransform()->Position(&temp);
	temp.x += offset.x; temp.y += offset.y;
	images[sheetNumber]->GetTransform()->Position(temp);*/
}

void Animation::SetUv(float x1, float y1, float x2, float y2, float offsetX, float offsetY)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	images[sheetNumber]->SetUv(x1,y1, x2,y2);
	//images[sheetNumber]->GetTransform()->Position(originalPos.x+offsetX, originalPos.y+offsetY, originalPos.z);
}

void Animation::SetScale(Vector2 inScale)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	for(auto i: images)
		i->SetScale(inScale);
	//images[sheetNumber]->SetScale(inScale);
}

void Animation::SetScale(float x, float y)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	for (auto i : images)
		i->SetScale(x, y);
	//images[sheetNumber]->SetScale(x, y);
}

void Animation::SetRotation(Vector3 Rotation)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	for (auto i : images)
		i->SetRotation(Rotation);
	images[sheetNumber]->SetRotation(Rotation);
}

void Animation::SetRotation(float x, float y, float z)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	for (auto i : images)
		i->SetRotation(x, y, z);
	images[sheetNumber]->SetRotation(x, y, z);
}

void Animation::SetRotationDegree(Vector3 Rotation)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	for (auto i : images)
		i->SetRotationDegree(Rotation);
	images[sheetNumber]->SetRotationDegree(Rotation);
}

void Animation::SetRotationDegree(float x, float y, float z)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	for (auto i : images)
		i->SetRotationDegree(x, y, z);
	images[sheetNumber]->SetRotationDegree(x, y, z);
}

void Animation::SetPosition()
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	if (datas[sheetNumber].empty())return;
	if (datas[sheetNumber].size()<= frameNumber)return;
	Vector2 scale = images[sheetNumber]->GetScale();
	Vector3 rot; images[sheetNumber]->GetTransform()->RotationDegree(&rot);
	if(int(rot.y)%360)
	images[sheetNumber]->SetPosition(
		originalPos.x - (datas[sheetNumber][frameNumber]->offset.x+sheetOffset[sheetNumber].x)*scale.x,
		originalPos.y + (datas[sheetNumber][frameNumber]->offset.y+sheetOffset[sheetNumber].y)*scale.y
	);
	else
		images[sheetNumber]->SetPosition(
			originalPos.x + (datas[sheetNumber][frameNumber]->offset.x + sheetOffset[sheetNumber].x)*scale.x,
			originalPos.y + (datas[sheetNumber][frameNumber]->offset.y + sheetOffset[sheetNumber].y)*scale.y
		);
}

void Animation::SetPosition(Vector2 Position)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	originalPos.x = Position.x;
	originalPos.y = Position.y;
}

void Animation::SetPosition(float x, float y)
{
	if (sheetNumber < 0)return;
	if (!images[sheetNumber])return;
	originalPos.x = x;
	originalPos.y = y;
}

void Animation::SetPerSecond(float perSecond)
{
	this->perSecond = perSecond;
}

void Animation::SetSheetOffset(UINT sheetNumber, float x, float y)
{
	sheetOffset[sheetNumber] = Vector2(x, y);
}

void Animation::SetStartNotify(UINT sheetNumber, UINT frameNumber, function<void()> notify)
{
	datas[sheetNumber][frameNumber]->notify_Start = notify;
}

void Animation::SetEndNotify(UINT sheetNumber, UINT frameNumber, function<void()> notify)
{
	datas[sheetNumber][frameNumber]->notify_End = notify;
}

void Animation::SetLerpTime(float lerpTime)
{
	this->lerpTime = lerpTime;
}

void Animation::DrawDebugLine(bool inBool)
{
	for(Sprite* i : images)
		i->DrawDebugLine(inBool);
}

void Animation::AddSprite(wstring spriteFileName)
{
	images.push_back(new Sprite(spriteFileName));
	AddSheetOffset();
}

void Animation::AddSprite(Texture * diffuseMap)
{
	images.push_back(new Sprite(diffuseMap));
	AddSheetOffset();
}

void Animation::AddFrameData(UINT sheetNumber,float sx, float sy, float ex, float ey,float offsetX,float offsetY)
{
	FrameData* temp = new FrameData(sx, sy, ex, ey, offsetX, offsetY);
	datas[sheetNumber].push_back(temp);
}

void Animation::AddFrameData(Vector2 start, Vector2 end, Vector2 offset)
{
	FrameData* temp = new FrameData(start, end, offset);
	datas[sheetNumber].push_back(temp);
}

void Animation::AddSheetOffset(float x, float y)
{
	if (sheetOffset.size() >= images.size()) return;
	sheetOffset.push_back(Vector2(x, y));
}

void Animation::AddSheetOffset(Vector2 offset)
{
	AddSheetOffset(offset.x, offset.y);
}

void Animation::ChangeSheetNumber(UINT InSheetNumber)
{
	if (InSheetNumber >= images.size())
		InSheetNumber = images.size() - 1;
	Vector3 vec;
	images[prevSheet]->GetTransform()->Position(&vec);
	images[InSheetNumber]->GetTransform()->Position(vec);
	prevSheet = sheetNumber;
	sheetNumber = InSheetNumber;
}

void Animation::Pass(UINT pass)
{
	for (UINT i = 0; i < images.size(); i++)
		images[i]->Pass(pass);
}

void Animation::Play(UINT sheetNumber)
{
	this->sheetNumber = sheetNumber;
	frameNumber = 0;
	prevSheet = sheetNumber;
	prevFrame = frameNumber;
	prevTime = Time::Get()->Running();
	SetPosition();
	SetUv(datas[sheetNumber][frameNumber]->start, datas[sheetNumber][frameNumber]->end, datas[sheetNumber][frameNumber]->offset);
}

void Animation::Stop()
{
	sheetNumber = -1;
}

void Animation::PlayLoop(UINT sheetNumber)
{
	SetLoop();
	Play(sheetNumber);
}

void Animation::PlayOnce(UINT sheetNumber)
{
	SetOnce();
	Play(sheetNumber);
}

void Animation::PlayOnceAndDisappear(UINT sheetNumber)
{
	SetOnceAndDisappear();
	Play(sheetNumber);
}

void Animation::PlayMontage(UINT sheetNumber)
{
	SetMontage();
	Play(sheetNumber);
	ExcuteStartNotify(sheetNumber,0);
}

void Animation::SetLoop()
{
	loop = 1;
	disappear = 0;
	montage = 0;
}

void Animation::SetOnce()
{
	loop = 0;
	disappear = 0;
	montage = 0;
}

void Animation::SetOnceAndDisappear()
{
	loop = 0;
	disappear = 1;
	montage = 0;
}

void Animation::SetMontage()
{
	if (montage)return;
	prev.loop = loop;loop = 0;
	prev.disappear = disappear; disappear = 0;
	prev.sheet = sheetNumber;
	prev.frame = frameNumber;
	montage = 1;
}

void Animation::EndMontage()
{
	//ExcuteEndNotify(sheetNumber,frameNumber);
	loop = prev.loop;
	disappear = prev.disappear;
	sheetNumber = prev.sheet;
	frameNumber = prev.frame;
	montage = 0;
}

void Animation::SetFrameNumb(UINT frameNumber)
{
	float curTime = Time::Get()->Running();
	prevTime = curTime;
	this->frameNumber = frameNumber;
	if (frameNumber >= datas[sheetNumber].size())
		loop ? frameNumber = 0 : sheetNumber = -1;
	if (sheetNumber < 0)return;
	SetUv(datas[sheetNumber][frameNumber]->start, datas[sheetNumber][frameNumber]->end, datas[sheetNumber][frameNumber]->offset);
	SetPosition();
}

void Animation::ReadFrameDatas(UINT sheetNumber)
{
	ifstream readFile;
	string str;
	str = String::ToString(images[sheetNumber]->GetSpriteFileName());
	while (str.back() != '.')
		str.pop_back();
	str += "txt";
	readFile.open("../../_Textures/"+str);
	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			string temp;
			getline(readFile, temp);
			if (temp == "")break;
			float input[6];			
			int blank = 0,idx=0;
			for (UINT i = idx; i < temp.size(); i++)
				if (temp[i] == ' ') 
				{ 
					input[idx++] = stof(temp.substr(blank, i - blank));
					blank = i + 1;
				}
			input[idx] = stof(temp.substr(blank));
			AddFrameData(sheetNumber, input[0], input[1], input[2], input[3], input[4], input[5]);
		}
	}
	readFile.close();
}

void Animation::ExcuteStartNotify(UINT sheetNumber, UINT frameNumber)
{
	if (datas[sheetNumber][frameNumber]->notify_Start == nullptr)return;
	datas[sheetNumber][frameNumber]->notify_Start();
}

void Animation::ExcuteEndNotify(UINT sheetNumber,UINT frameNumber)
{
	if (datas[sheetNumber][frameNumber]->notify_End == nullptr)return;
	datas[sheetNumber][frameNumber]->notify_End();
}
