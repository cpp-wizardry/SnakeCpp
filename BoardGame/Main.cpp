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
    MSG msg = { };

    while (input != 'x')
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                input = 'x';
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        board.renderBoard();

        std::cout << "\ndeplace toi avec zqsd\n";
        std::cout << "Ton Score:" << snake->getScore();
    }

    std::cout << "partie fini" << std::endl;
    return 0;
}
