#include "board.hpp"
#include "stdafx.h"
#include <conio.h>
#include "fenetre.hpp"

int main() {
	snake::Board board;

	snake::Fenetre::CreateMainWindow(&board);

	std::shared_ptr<snake::Snake> snake = board.getSnake();
	if (!snake)
	{
		std::cout << "debug: snake pas trouvé\n\r";
		return 1;
	}

	board.spawnFruit();

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	std::cout << "partie fini" << std::endl;
	return 0;
}