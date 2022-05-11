#include "stdafx.h"
#include "Editor.h"

void Editor::Initialize()
{
	openFunc = bind(&Editor::OpenFile, this,placeholders::_1);
	sprite= new Sprite(L"Character/uppertest.png");
	sprite->DrawDebugLine(true);
	ani = new Animation();
	ani2 = new Animation();
	ani->AddSprite(L"character/uppertest.png");
	ani2->AddSprite(L"character/uppertest.png");
}

void Editor::Destroy()
{
	SafeDelete(sprite);
	SafeDelete(texture);
}

void Editor::Update()
{
	if (ImGui::Button("Open Sprite File"))
	{
		Path::OpenFileDialog(spriteFileName, Path::ImageFilter, spriteFileDirectory, openFunc);
		SafeDelete(texture);
		texture = new Texture(spriteFileDirectory + spriteFileName);
		sprite->SetTexture(spriteFileDirectory + spriteFileName);
		Context::Get()->GetCamera()->Position(0,0,0);
		width = texture->GetWidth(); height = texture->GetHeight();
	}
	if (ImGui::Button("Save This Sprite To PNG ver.1"))
	{
		SaveFile(0);
	}
	if (ImGui::Button("Save This Sprite To PNG ver.2"))
	{
		SaveFile(1);
	}
	if (ImGui::Button("Open Animation File"))
	{
		Path::OpenFileDialog(spriteFileName, Path::ImageFilter, spriteFileDirectory, openFunc);
		SafeDelete(texture);
		texture = new Texture(spriteFileDirectory + spriteFileName);
		SafeDelete(ani);
		ani = new Animation(spriteFileDirectory + spriteFileName);
		Context::Get()->GetCamera()->Position(0, 0, 0);
		width = texture->GetWidth(); height = texture->GetHeight();
	}
	//Uv오프셋
	ImGui::Separator();
	ImGui::InputInt("UOffset", &offsetU);
	ImGui::InputInt("VOffset", &offsetV);
	//이미지 크기 결정
	ImGui::Separator();
	ImGui::InputInt("Width", &width);
	ImGui::InputInt("Height", &height);
	if (width < 1) width = 1;
	if (height < 1) height = 1;
	//이미지 이름
	ImGui::InputText("FileName(128)", &saveFileName[0], 128);
	//프레임 UV조절
	if (ImGui::Button("Add FrameData"))
	{
		AddFrameUV();
	}
	ImGui::InputInt("FrameNumber", &frameNumber);
	ImGui::InputInt("Xoffset", &offsetX);
	ImGui::InputInt("Yoffset", &offsetY);
	if (ImGui::Button("Adujst FrameData"))
	{
		SetFrameUV();
	}
	if (ImGui::Button("Adujst Offset"))
	{
		SetFrameOffset();
	}
	if (ImGui::Button("Save FrameData"))
	{
		SaveFrameData();
	}
	if (ImGui::Button("Load FrameData"))
	{
		ani->ReadFrameDatas(0);
	}
	ImGui::Separator();
	//테두리
	if (ImGui::Button("Sprite"))play = 0;
	if (ImGui::Button("Animation"))play = 1;
	ImGui::SliderFloat("Frame/Second", &perSecond, 1.0f, 30.0f);
	ImGui::SliderFloat("ScaleX", &scaleX, 1.0f, 10.0f);
	ImGui::SliderFloat("ScaleY", &scaleY, 1.0f, 10.0f);

	static int pass=0;
	ImGui::SliderInt("Pass", &pass, 0, 1);

	sprite->SetUv(offsetU, offsetV, offsetU + width, offsetV + height);
	sprite->Update();
	sprite->Pass(pass);
	ani->SetPerSecond(perSecond);
	ani->SetScale(scaleX, scaleY);
	ani->Update();
	ani->Pass(pass);
	
	ImGui::Begin("Animation");
	if (ImGui::Button("Add Sheet"))
	{
		Path::OpenFileDialog(spriteFileName, Path::ImageFilter, spriteFileDirectory, openFunc);
		ani->AddSprite(spriteFileDirectory + spriteFileName);
	}
	if (ImGui::Button("Read Sheet Data"))
		ani->ReadFrameDatas(ani->GetSheetSize() - 1);
	ImGui::SliderInt("SheetNumber", &sheetnumber, 0, ani->GetSheetSize() - 1);
	if (ImGui::Button("Play"))
		ani->Play(sheetnumber);
	ImGui::InputInt("Xoffset", &AnimOffsetX);
	ImGui::InputInt("Yoffset", &AnimOffsetY);
	if (ImGui::Button("Adujst Offset"))
		ani->SetSheetOffset(sheetnumber, AnimOffsetX, AnimOffsetY);
	if (ImGui::Button("Second Aimation"))
		second = !second;
	if (second)
	{
		if (ImGui::Button("Open Animation File2"))
		{
			Path::OpenFileDialog(spriteFileName, Path::ImageFilter, spriteFileDirectory, openFunc);
			SafeDelete(texture);
			texture = new Texture(spriteFileDirectory + spriteFileName);
			SafeDelete(ani2);
			ani2 = new Animation(spriteFileDirectory + spriteFileName);
			Context::Get()->GetCamera()->Position(0, 0, 0);
			width = texture->GetWidth(); height = texture->GetHeight();
		}
		if (ImGui::Button("Add Sheet2"))
		{
			Path::OpenFileDialog(spriteFileName, Path::ImageFilter, spriteFileDirectory, openFunc);
			ani2->AddSprite(spriteFileDirectory + spriteFileName);
		}
		if (ImGui::Button("Read Sheet Data2"))
			ani2->ReadFrameDatas(ani2->GetSheetSize() - 1);
		ImGui::SliderInt("Sheet2Number", &sheetnumber2, 0, ani2->GetSheetSize() - 1);
		if (ImGui::Button("Play2"))
			ani2->Play(sheetnumber2);
		ImGui::InputInt("Xoffset2", &AnimOffsetX2);
		ImGui::InputInt("Yoffset2", &AnimOffsetY2);
		if (ImGui::Button("Adujst Offset2"))
			ani2->SetSheetOffset(sheetnumber2, AnimOffsetX2, AnimOffsetY2);
	}
	ImGui::End();
	ani2->Update();
}

void Editor::Render()
{
	if(!play) sprite->Render();
	else ani->Render();
	if (second)ani2->Render();
}

void Editor::OpenFile(wstring fileName)
{
	spriteFileName = Path::GetFileName(fileName);
	spriteFileDirectory = Path::GetDirectoryName(fileName);
	UINT idx = spriteFileDirectory.find(L"_Textures")+10;
	spriteFileDirectory= spriteFileDirectory.substr(idx);
}

void Editor::SaveFile(bool reverse)
{
	if (width < 2 || height < 2)
	{
		MessageBox(D3D::Get()->GetDesc().Handle,
			L"Sprite size is too small.\r\nPlz adjust size.", L"Warning",
			MB_OK);
		return;
	}
	if (texture == nullptr)
	{
		MessageBox(D3D::Get()->GetDesc().Handle,
			L"Choose Sprite first", L"Warning",
			MB_OK);
		return;
	}
	/*char ch = saveFileName[0];
	if (!ch)
	{
		MessageBox(D3D::Get()->GetDesc().Handle,
			L"Set Sprite name first", L"Warning",
			MB_OK);
		return;
	}*/
	//원본Desc복사
	ID3D11Texture2D* srcTexture = texture->GetTexture();
	D3D11_TEXTURE2D_DESC srcDesc;
	srcTexture->GetDesc(&srcDesc);
	//원본복사
	ID3D11Texture2D* readTexture;
	D3D11_TEXTURE2D_DESC readDesc;
	ZeroMemory(&readDesc, sizeof(D3D11_TEXTURE2D_DESC));
	readDesc.Width = srcDesc.Width;
	readDesc.Height = srcDesc.Height;
	readDesc.Format = srcDesc.Format;
	readDesc.SampleDesc = srcDesc.SampleDesc;
	readDesc.ArraySize = 1;
	readDesc.MipLevels = 1;
	readDesc.Usage = D3D11_USAGE_STAGING;
	readDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	Check(D3D::GetDevice()->CreateTexture2D(&readDesc, nullptr, &readTexture));
	D3D::GetDC()->CopyResource(readTexture, srcTexture);
	UINT _32=0;
	if (readDesc.Width % 32)
	_32 = 32 - readDesc.Width % 32;
	UINT* pixels = new UINT[(readDesc.Width+ _32) * readDesc.Height];
	UINT* results = new UINT[width * height];
	D3D11_MAPPED_SUBRESOURCE subResouce;
	D3D::GetDC()->Map(readTexture, 0, D3D11_MAP_READ, 0, &subResouce);
	{
		memcpy(pixels, subResouce.pData, sizeof(UINT) * (readDesc.Width + _32) * readDesc.Height);
	}
	D3D::GetDC()->Unmap(readTexture, 0);
	for (int y = 0; y < height; y++)
	{
		UINT offset = ((offsetV + y) * (readDesc.Width + _32)) + offsetU;
		for (int x = 0; x < width; x++)
		{
			UINT pixel = pixels[offset+ x];
			results[y*width + x] = 0;
			UINT a = (pixel & 0xFF000000) >> 24;
			UINT b = (pixel & 0x00FF0000) >> 16;
			UINT g = (pixel & 0x0000FF00) >> 8;
			UINT r = (pixel & 0x000000FF);
			results[y*width + x] += a<<24;
			if (b == 0 && g == 248 && r == 0)
			{
				results[y*width + x] = 0;
				continue;
			}
			if (b == 248 && g == 0&& r == 248)
			{
				results[y*width + x] = 0;
				continue;
			}
			if (!reverse)
			{
				results[y*width + x] += r << 16;
				results[y*width + x] += g << 8;
				results[y*width + x] += b;
			}
			else
			{
				results[y*width + x] += b << 16;
				results[y*width + x] += g << 8;
				results[y*width + x] += r;
			}
		}
	}
	ID3D11Texture2D* saveTexture;
	D3D11_TEXTURE2D_DESC saveDesc;
	ZeroMemory(&saveDesc, sizeof(D3D11_TEXTURE2D_DESC));
	saveDesc.Width = width;
	saveDesc.Height = height;
	saveDesc.ArraySize = 1;
	saveDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	saveDesc.MipLevels = 1;
	saveDesc.SampleDesc = readDesc.SampleDesc;
	saveDesc.Usage = D3D11_USAGE_STAGING;
	saveDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA saveSubResource;
	saveSubResource.pSysMem = results;
	saveSubResource.SysMemPitch = sizeof(UINT)*width;
	saveSubResource.SysMemSlicePitch = sizeof(UINT)*height*width;
	Check(D3D::GetDevice()->CreateTexture2D(&saveDesc, &saveSubResource, &saveTexture));

	wstring test = L"../../_Textures/"+spriteFileDirectory + Path::GetFileNameWithoutExtension(spriteFileName)+ L"_Edit.png";
	Check(D3DX11SaveTextureToFile(D3D::GetDC(), saveTexture, D3DX11_IFF_PNG, test.c_str()));

	SafeRelease(srcTexture);
	SafeRelease(readTexture);
	SafeRelease(saveTexture);

	SafeDeleteArray(pixels);
	SafeDeleteArray(results);
	MessageBox(D3D::Get()->GetDesc().Handle,
		L"Work complete", L"^__^",
		MB_OK);
}

void Editor::SaveFrameData()
{
	if (ani[0].GetFrameDatas().empty())
	{
		MessageBox(D3D::Get()->GetDesc().Handle,
			L"There is no Datas..", L"Warning",
			MB_OK);
		return;
	}
	string str = "../../_Textures/"+String::ToString(spriteFileDirectory)+String::ToString(Path::GetFileNameWithoutExtension(spriteFileName)) + ".txt";
	ofstream out;
	out.open(str);
	for (int i = 0; i < ani[0].GetFrameDatas().size(); i++)
	{
		Vector2 start = ani[0].GetFrameDatas()[i]->start;
		Vector2 end = ani[0].GetFrameDatas()[i]->end;
		Vector2 offset = ani[0].GetFrameDatas()[i]->offset;
		out << start.x << " " << start.y << " " << end.x << " " << end.y << " " << offset.x << " " << offset.y << "\n";
	}
	out.close();
}

void Editor::AddFrameUV()
{
	ani->AddFrameData(0,offsetU, offsetV, offsetU + width, offsetV + height);
}

void Editor::SetFrameUV()
{
	if(ani->GetFrameDatas().size()<=frameNumber)
	{
		MessageBox(D3D::Get()->GetDesc().Handle,
			L"FameNumber is out of bounds.", L"Warning",
			MB_OK);
		return;
	}
	ani->GetFrameDatas()[frameNumber]->start = Vector2(offsetU, offsetV);
	ani->GetFrameDatas()[frameNumber]->end = Vector2(offsetU + width, offsetV + height);
}

void Editor::SetFrameOffset()
{
	if (ani->GetFrameDatas().size() <= frameNumber)
	{
		MessageBox(D3D::Get()->GetDesc().Handle,
			L"FameNumber is out of bounds.", L"Warning",
			MB_OK);
		return;
	}
	ani->GetFrameDatas()[frameNumber]->offset = Vector2(offsetX, offsetY);
}
