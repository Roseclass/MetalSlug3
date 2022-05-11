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

//TODO : �������̽�, ĳ���� ���, ��Ȱ, ���ⱳü, ���� ��Ʈ�ڽ������, �̻����߰�, ������ε�, ���ѱ��
//Done : howtoplay-���,select,crab,�׶��� ������,�ݶ��̴�,��Ƽ����,Bullet,ĳ���� ����,�����Ʈ ������Ʈ,�ݶ��̴� �Ŵ���
//��������Ʈ������,�����ӵ������а���,���ε������а���
//Monster : Ai
//Addtive : Ÿ��Ʋ��, ������
//�ҷ�  IDamage �浹 ó��