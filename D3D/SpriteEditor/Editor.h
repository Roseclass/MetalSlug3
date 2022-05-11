#pragma once
#include "Systems/IExecute.h"

class Editor : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};
	virtual void SetMain(IExecute * main) override {};
	virtual void Start() override {};

private:
	Texture* texture;
	Sprite* sprite;
	wstring spriteFileDirectory;
	wstring spriteFileName;
	vector<Vector2> uvs;

	function<void(wstring)> openFunc;
	void OpenFile(wstring fileName);	

	function<void(wstring)> saveFunc;
	void SaveFile(bool reverse);
	void SaveFrameData();
	void AddFrameUV();
	void SetFrameUV();
	void SetFrameOffset();

	//imgui
	int width = 1;
	int height = 1;
	int offsetU = 0;
	int offsetV = 0;
	int offsetX = 0;
	int offsetY = 0;
	int AnimOffsetX = 0;
	int AnimOffsetY = 0;
	int AnimOffsetX2 = 0;
	int AnimOffsetY2 = 0;
	string saveFileName;
	int frameNumber = 0;
	int sheetnumber = 0;
	int sheetnumber2 = 0;
	int play = 0;
	int second = 0;
	float perSecond = 10.0f;
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	Animation* ani;
	Animation* ani2;
};

//TODO : 인터페이스, 캐릭터 사망, 부활, 무기교체, 보스 히트박스만들기, 미사일추가, 쓰레드로딩, 씬넘기기
//Done : howtoplay-배경,select,crab,그라운드 에디터,콜라이더,노티파이,Bullet,캐릭터 세팅,무브먼트 컴포넌트,콜라이더 매니저
//스프라이트에디터,프레임데이터읽고쓰기,라인데이터읽고쓰기
//Monster : Ai
//Addtive : 타이틀씬, 보스전
//불렛  IDamage 충돌 처리