//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "illuminationProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	IlluminationProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"04 - Illuminazione e Shading");
	app.setWindowSize(640, 480);

	return app.run();
}
