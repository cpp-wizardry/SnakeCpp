#include "Fenetre.h"
namespace Fenetre {




    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
    
        switch (uMsg)
        {
        case WM_DESTROY:
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
        }
        break;

        case WM_KEYDOWN:
        {
            Board* board = (Board*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (board)
            {
                switch (wParam)
                {
                case 'Q': board->moveSnake(1); break;
                case 'Z': board->moveSnake(2); break;
                case 'D': board->moveSnake(3); break;
                case 'S': board->moveSnake(4); break;
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

                if (std::dynamic_pointer_cast<Snake>(entity))
                {
                    brush = CreateSolidBrush(RGB(0, 0, 255)); 

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


