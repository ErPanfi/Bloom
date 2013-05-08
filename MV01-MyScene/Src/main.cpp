//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "BloomProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	BloomProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"Tentativo di far mescolare le texture");
	app.setWindowSize(640, 480);

	return app.run();
}
