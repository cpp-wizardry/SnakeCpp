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

	int squareWidth = clientRect.right / boardSize;
	int squareHeight = clientRect.bottom / boardSize;

	for (int row = 0; row < boardSize; ++row)
	{
		for (int col = 0; col < boardSize; ++col)
		{
			RECT square = {
				col * squareWidth,
				row * squareHeight,
				(col + 1) * squareWidth,
				(row + 1) * squareHeight
			};

			Index index { col, row };
			auto entity = board->getEntityAt(index);

			HBRUSH brush = nullptr;
			if (entity == Entity::Kind::snake)
			{
				int segmentOrder = board->snake.getSnakeSegmentOrder(index);
				int snakeLength = board->snake.size();

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
					brush = CreateSolidBrush(RGB(0, 0, 255));
				}
			}
			else if (entity == Entity::Kind::fruit)
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

			int segmentOrder = board->snake.getSnakeSegmentOrder(index);
			if (segmentOrder != -1) {
				std::wstring text = std::to_wstring(segmentOrder);
				DrawText(hdc, text.c_str(), -1, &square, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}
}

} // namespace snake::win32