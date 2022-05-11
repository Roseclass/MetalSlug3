#pragma once

#include "Framework.h"
#pragma comment(lib, "../Debug/Framework.lib")

#ifdef _DEBUG

#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

#endif // _DEBUG
