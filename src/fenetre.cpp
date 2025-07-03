#include "fenetre.h"
#include <chrono>
#define TIMER_ID 1


using namespace std::chrono;
namespace Fenetre {

    int frames = 0;
    int fps = 0;
    auto lastTime = steady_clock::now();

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
    
        switch (uMsg)
        {
        case WM_DESTROY:
            KillTimer(hwnd, TIMER_ID);
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            Board* board = (Board*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (board)
            {
                WNDRenderBoard(hwnd, hdc, *board);
                frames++;

                auto currentTime = steady_clock::now();
                auto elapsed = duration_cast<seconds>(currentTime - lastTime).count();

                if (elapsed >= 1) {
                    fps = frames;
                    frames = 0;
                    lastTime = currentTime;


                    std::cout << "FPS: " << fps << std::endl;
                    std::wstring fpsText = L"FPS: " + std::to_wstring(fps);
                    std::wstring title = L"Snake - FPS: " + std::to_wstring(fps);
                    SetWindowText(hwnd, title.c_str());
                    TextOut(hdc, 10, 10, fpsText.c_str(), int(fpsText.length()));
                }
            }
            //HBRUSH CustomBrush = CreateSolidBrush(50);//def de la couleur du pinceau
            //FillRect(hdc, &ps.rcPaint, CustomBrush);//application de la peinture sur le background
            //DeleteObject(CustomBrush);//nettoyage du pinceau
       
            EndPaint(hwnd, &ps);
        }
        break;
        case WM_CREATE:
        {
            LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
            Board* board = (Board*)pcs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)board);
            auto speed = board->getSnake()->getSpeed();
            SetTimer(hwnd, TIMER_ID, UINT(50 - speed), NULL); // todo: round?

        }
        break;
        case WM_KEYDOWN:
        {
            Board* board = (Board*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            auto snake = board->getSnake();

            int desiredDirection = snake->getCurrentDirection();

            switch (wParam)
            {
            case 'Q': desiredDirection = 1; break;
            case 'Z': desiredDirection = 2; break;
            case 'D': desiredDirection = 3; break;
            case 'S': desiredDirection = 4; break;
            }

            if (!snake->isOpposite(snake->getNextDirection(), desiredDirection))
            {
                snake->setNextDirection(desiredDirection);
            }
        }
        break;

        case WM_TIMER:
        {
            Board* board = (Board*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (board)
            {
                auto snake = board->getSnake();
                if (snake->getAlive())
                {
                    board->moveSnake(snake->getNextDirection());

                    snake->setCurrentDirection(snake->getNextDirection());

                    InvalidateRect(hwnd, NULL, TRUE);
                }
                else
                {
                    KillTimer(hwnd, TIMER_ID);
                }
            }
        }
        break;

        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    HWND CreateMainWindow(Board *board)
    {
        const wchar_t CLASS_NAME[] = L"SnakeWindow";

        WNDCLASS wc = {};
        wc.lpfnWndProc = Fenetre::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = CLASS_NAME;

        RegisterClass(&wc);

        HWND hwnd = CreateWindowEx(
            0, CLASS_NAME, L"Snake", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, GetModuleHandle(NULL), board
        );

        if (hwnd == NULL) {
            std::cout << "debug: echec de creation de la fenetre\n";
            exit(1);
        }

        ShowWindow(hwnd, SW_SHOW);
        return hwnd;
    }

    void Fenetre::WNDRenderBoard(HWND hwnd, HDC hdc, const Board& board)
    {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        int squareWidth = clientRect.right / Board::getBoardSize();
        int squareHeight = clientRect.bottom / Board::getBoardSize();

        for (int row = 0; row < Board::getBoardSize(); ++row)
        {
            for (int col = 0; col < Board::getBoardSize(); ++col)
            {
                RECT square = {
                    col * squareWidth,
                    row * squareHeight,
                    (col + 1) * squareWidth,
                    (row + 1) * squareHeight
                };

                int index = row * Board::getBoardSize() + col;
                std::shared_ptr<Entity> entity = board.getEntityAt(index);


                HBRUSH brush = nullptr;
                auto snakeLength = board.getSnake()->getBody().size();
                if (std::dynamic_pointer_cast<Snake>(entity))
                {
                    int segmentOrder = board.getSnakeSegmentOrder(index);
                    int snakeLength = int(board.getSnake()->getBody().size());

                    if (segmentOrder != -1 && snakeLength > 1)
                    {
                        float ratio = static_cast<float>(segmentOrder) / (snakeLength - 1);

                        int brightness = static_cast<int>(ratio * 200); 

                        int red = brightness;
                        int green = brightness;
                        int blue = 255;

                        brush = CreateSolidBrush(RGB(red, green, blue));
                    }
                    else
                    {
                        brush = CreateSolidBrush(RGB(0, 0, 255)); //bleu si ça bug
                    }
                }
                else if (std::dynamic_pointer_cast<Fruit>(entity))
                {
                    brush = CreateSolidBrush(RGB(255, 0, 0));
                }
                else
                {
                    brush = CreateSolidBrush(RGB(0, 255, 0)); 
                }

                FillRect(hdc, &square, brush);
                DeleteObject(brush);
                SetBkMode(hdc, TRANSPARENT);


                int segmentOrder = board.getSnakeSegmentOrder(index);
                if (segmentOrder != -1) {
                    std::wstring text = std::to_wstring(segmentOrder);
                    DrawText(hdc, text.c_str(), -1, &square, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }


            }
        }
    }

    }


