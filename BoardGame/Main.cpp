#include "Board.h"
#include "stdafx.h"
#include <conio.h>
#include "Fenetre.h"


int main() {

    Board board;

    HWND hwnd = Fenetre::CreateMainWindow(&board);
    board.setWindowHandle(hwnd);

    std::shared_ptr<Snake> snake = board.getSnake();
    if (!snake)
    {
        std::cout << "debug: snake pas trouvé\n\r";
        return 1;
    }

    board.spawnFruit();

    char input = ' ';

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    std::cout << "partie fini" << std::endl;
    return 0;
}
