#pragma once
#include "Systems/IExecute.h"

class LineEdit
{
public:
	LineEdit(Line* pLine, string fileName);
	~LineEdit();
	void Update();
	void SaveFile();
	void LoadFile();
private:
	Line* line;
	string fileName;
	bool debug=0;
};