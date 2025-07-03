#pragma once
#include "stdafx.h"
#include "board.hpp"
#include "fruit.hpp"

namespace snake::Fenetre {

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND CreateMainWindow(Board* board);
void WNDRenderBoard(HWND hwnd, HDC hdc, const Board& board);

}