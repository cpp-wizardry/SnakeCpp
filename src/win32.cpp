#include "stdafx.h"

#include <chrono>

#include "board.hpp"
#include "win32.hpp"

namespace snake::win32 {

constexpr UINT_PTR TIMER_ID = 1;

namespace {
	Board* board = nullptr;
	int frames = 0;
	int fps = 0;
	auto lastTime = std::chrono::steady_clock::now();
	std::wstring fpsText;
}

void WNDRenderBoard(HWND hwnd, HDC hdc);

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

		WNDRenderBoard(hwnd, hdc);
		frames++;

		auto currentTime = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();

		if (elapsed >= 1) {
			fps = frames;
			frames = 0;
			lastTime = currentTime;

			fpsText = L"FPS: " + std::to_wstring(fps);
			std::wstring title = L"Snake - FPS: " + std::to_wstring(fps);
			SetWindowText(hwnd, title.c_str());
		}
		TextOut(hdc, 10, 10, fpsText.c_str(), int(fpsText.length())); // todo: show every frame

		EndPaint(hwnd, &ps);
	}
		return 0;

	case WM_CREATE:
	{
		SetTimer(hwnd, TIMER_ID, UINT(50 - board->snake.speed), NULL); // todo: round?
	}
		return 0;

	case WM_KEYDOWN:
	{
		auto& snake = board->snake;
		auto desiredDirection = snake.direction;

		switch (wParam)
		{
#ifdef SNAKE_USE_CURSOR_KEYS
		case VK_LEFT : desiredDirection = Direction::left ; break;
		case VK_UP   : desiredDirection = Direction::up   ; break;
		case VK_RIGHT: desiredDirection = Direction::right; break;
		case VK_DOWN : desiredDirection = Direction::down ; break;
#else
		case 'Q': desiredDirection = Direction::left ; break;
		case 'Z': desiredDirection = Direction::up   ; break;
		case 'D': desiredDirection = Direction::right; break;
		case 'S': desiredDirection = Direction::down ; break;
#endif
		}

		if (!isOpposite(snake.direction, desiredDirection))
		{
			snake.direction = desiredDirection;
		}
	}
		return 0;

	case WM_TIMER:
	{
		board->moveSnake();
		InvalidateRect(hwnd, NULL, TRUE);
	}
		return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND CreateMainWindow(Board& board_)
{
	board = &board_;

	const wchar_t CLASS_NAME[] = L"SnakeWindow";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0, CLASS_NAME, L"Snake", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, GetModuleHandle(NULL), NULL
	);

	if (hwnd != NULL) {
		ShowWindow(hwnd, SW_SHOW);
	}

	return hwnd;
}

void WNDRenderBoard(HWND hwnd, HDC hdc)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	int const squareWidth = clientRect.right / boardSize;
	int const squareHeight = clientRect.bottom / boardSize;

	// assume mostly empty and accept larger overdraw as snake grows
	{
		auto const brush = CreateSolidBrush(RGB(0, 255, 0));
		FillRect(hdc, &clientRect, brush);
		DeleteObject(brush);
	}

	// fruit
	{
		auto const brush = CreateSolidBrush(RGB(255, 0, 0));
		Pos const p = board->fruit.pos;
		RECT r = {
			p.x * squareWidth,
			p.y * squareHeight,
			(p.x + 1) * squareWidth,
			(p.y + 1) * squareHeight
		};
		FillRect(hdc, &r, brush);
		DeleteObject(brush);
	}

	// snake
	for (int i = 0, len = board->snake.size(); i < len; ++i) {
		HBRUSH brush = nullptr;
		if (len > 1)
		{
			float const ratio = static_cast<float>(i) / (len - 1);
			int const brightness = static_cast<int>(ratio * 200); 

			int const red = brightness;
			int const green = brightness;
			int const blue = 255;

			brush = CreateSolidBrush(RGB(red, green, blue));
		}
		else
		{
			brush = CreateSolidBrush(RGB(0, 0, 255));
		}

		Pos const p = board->snake.body[i];
		RECT square = {
			p.x * squareWidth,
			p.y * squareHeight,
			(p.x + 1) * squareWidth,
			(p.y + 1) * squareHeight
		};

		FillRect(hdc, &square, brush);
		DeleteObject(brush);

		std::wstring text = std::to_wstring(i);
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, text.c_str(), -1, &square, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

} // namespace snake::win32