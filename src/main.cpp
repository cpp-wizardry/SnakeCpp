#include "stdafx.h"

#include "board.hpp"
#include "win32.hpp"

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/, int /*nCmdShow*/)
{
	snake::Board board;
	if (snake::win32::CreateMainWindow(board) == NULL)
	{
		return 1;
	}

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}