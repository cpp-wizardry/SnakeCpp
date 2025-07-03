#include "stdafx.h"

#include "board.hpp"
#include "fenetre.hpp"

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/, int /*nCmdShow*/)
{
	snake::Board board;

	if (snake::Fenetre::CreateMainWindow(&board) == NULL)
	{
		return 1;
	}

	std::shared_ptr<snake::Snake> snake = board.getSnake();
	if (!snake)
	{
		return 1;
	}

	board.spawnFruit();

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}