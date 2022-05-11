#include "stdafx.h"
#include "LineEdit.h"
#include <fstream>

LineEdit::LineEdit(Line* pLine,string fileName)
{
	line = pLine;
	this->fileName = fileName;
}

LineEdit::~LineEdit()
{
	line = nullptr;
}

void LineEdit::Update()
{
	return;
	ImGui::Begin(fileName.c_str());
	if (ImGui::Button("Debug"))debug = !debug;
	if (!debug)
	{
		ImGui::End();
		return;
	}
	//lineDraw
	if (Mouse::Get()->Down(0)&&Keyboard::Get()->Press(VK_CONTROL))
	{
		Vector3 pos;
		Context::Get()->GetViewport()->GetMousePos(&pos);
		line->AddVertex(Vector2(pos.x, pos.y));
	}
	if (Mouse::Get()->Down(1))
	{
		line->DeleteVertex();
	}
	if (ImGui::Button("SaveLine"))
	{
		SaveFile();
	}
	if (ImGui::Button("LoadLine"))
	{
		LoadFile();
	}
	ImGui::End();
}

void LineEdit::SaveFile()
{
	string str = "../MapData/" + fileName + ".txt";
	ofstream out;
	out.open(str);
	for (int i = 0; i < line->Count(); i++)
	{
		Vector2 vec;
		if (line->GetVertexPosition(i, &vec) == 0)
		{
			out.close();
			return;
		}
		out << vec.x << " " << vec.y << "\n";
	}
	out.close();
}

void LineEdit::LoadFile()
{
	ifstream readFile;
	string str = "../MapData/" + fileName + ".txt";
	readFile.open(str);
	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			string temp;
			getline(readFile, temp);
			if (temp == "")break;
			float input[2];
			int blank = 0, idx = 0;
			for (UINT i = idx; i < temp.size(); i++)
				if (temp[i] == ' ')
				{
					input[idx++] = stof(temp.substr(blank, i - blank));
					blank = i + 1;
				}
			input[idx] = stof(temp.substr(blank));
			line->AddVertex(Vector2(input[0], input[1]));
		}
	}
	readFile.close();
}
