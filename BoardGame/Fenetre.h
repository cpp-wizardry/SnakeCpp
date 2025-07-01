#pragma once
#include "stdafx.h"
#include "board.h"

#include "Fruit.h"
namespace Fenetre {

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND CreateMainWindow(Board* board);
    void WNDRenderBoard(HWND hwnd, HDC hdc, const Board& board);
}
