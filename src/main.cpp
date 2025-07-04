#include "stdafx.h"

#include "board.hpp"

namespace snake {

namespace {

Board board;
int width = 0;
int height = 0;

HDC backDC = nullptr;
HBITMAP backBuf = nullptr;

int frames = 0;

using clock = std::chrono::steady_clock;

std::chrono::milliseconds periodUpdate;
clock::time_point lastUpdateTime;

constexpr std::chrono::milliseconds periodDraw { 16 };
clock::time_point lastDrawTime;

clock::time_point lastFPSUpdateTime;
float fps = 0;
std::wstring fpsText;

void draw(HDC hdc, float dt)
{
	int const squareWidth  = width  / boardSize;
	int const squareHeight = height / boardSize;

	// assume mostly empty and accept larger overdraw as snake grows
	{
		auto const brush = CreateSolidBrush(RGB(0, 255, 0));
		RECT rect { 0, 0, width, height };
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
	}

	// fruit
	{
		auto const brush = CreateSolidBrush(RGB(255, 0, 0));
		Pos const p = board.fruit.pos;
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
	for (int i = 0, len = int(board.snake.bodyPrev.size()); i < len; ++i) {
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

		// todo: wrapping from left to right draws the entire rect on the right
		// when warping there should be 2 rect draws to show partially on both sides
		Pos const p[2] { board.snake.body[i], board.snake.bodyPrev[i] };
		float dx = std::fmodf((p[0].x - p[1].x)*squareWidth  + width /2.0f, float(width )) - width /2.0f;
		float dy = std::fmodf((p[0].y - p[1].y)*squareHeight + height/2.0f, float(height)) - height/2.0f;
		if (dx < -width /2.0f) dx += width;
		if (dy < -height/2.0f) dy += height;
		int x = int(std::fmodf(p[1].x*squareWidth  + dx*dt + width , float(width )));
		int y = int(std::fmodf(p[1].y*squareHeight + dy*dt + height, float(height)));
		RECT rect = {
			x, y,
			x + squareWidth, y + squareHeight
		};

		FillRect(hdc, &rect, brush);
		DeleteObject(brush);

		std::wstring text = std::to_wstring(i);
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	TextOut(hdc, 10, 10, fpsText.c_str(), int(fpsText.length()));
}

void update()
{
	auto const now = clock::now();
	if (now - lastUpdateTime >= periodUpdate)
	{
		board.moveSnake();
		lastUpdateTime = now;
	}
}

void sleepTargetDrawPeriod()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		periodUpdate = std::chrono::milliseconds{ 50 - int(board.snake.speed) };
		return 0;

	case WM_DESTROY:
		if (backDC) DeleteDC(backDC);
		if (backBuf) DeleteObject(backBuf);
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
	{
		width = LOWORD(lParam);
		height = HIWORD(lParam);

		auto hdc = GetDC(hwnd);
		if (backDC) DeleteDC(backDC);
		backDC = CreateCompatibleDC(hdc);

		if (backBuf) DeleteObject(backBuf);
		backBuf = CreateCompatibleBitmap(hdc, width, height);

		SelectObject(backDC, backBuf);
		ReleaseDC(hwnd, hdc);
	}
		return 0;

	case WM_PAINT:
	{
		auto now = clock::now();
		PAINTSTRUCT ps;
		auto hdc = BeginPaint(hwnd, &ps);
		auto const dt = std::chrono::duration<float>(now - lastUpdateTime).count() / std::chrono::duration<float>(periodUpdate).count();
		draw(backDC, dt);
		BitBlt(hdc, 0, 0, width, height, backDC, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		lastDrawTime = clock::now();

		frames++;

		float elapsed = std::chrono::duration<float>(lastDrawTime - lastFPSUpdateTime).count();
		if (elapsed >= 1) {
			fps = float(frames) / elapsed;
			frames = 0;
			lastFPSUpdateTime = lastDrawTime;

			fpsText = L"FPS: " + std::to_wstring(fps);
			std::wstring title = L"Snake - FPS: " + std::to_wstring(fps);
			SetWindowText(hwnd, title.c_str());
		}
	}
		return 0;

	case WM_KEYDOWN:
	{
		auto& snake = board.snake;
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

		if (!isColinear(snake.direction, desiredDirection))
		{
			snake.direction = desiredDirection;
		}
	}
		return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND createMainWindow(WNDPROC proc)
{
	const wchar_t CLASS_NAME[] = L"SnakeWindow";

	WNDCLASS wc = {};
	wc.lpfnWndProc = proc;
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

} // namespace

} // namespace snake

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/, int /*nCmdShow*/)
{
	auto hwnd = snake::createMainWindow(snake::windowProc);
	if (!hwnd)
		return 1;

	for (;;)
	{
		for (MSG msg {}; PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE); )
		{
			if (msg.message == WM_QUIT)
				goto L_quit;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		snake::update();

		InvalidateRect(hwnd, nullptr, FALSE);
		UpdateWindow(hwnd);

		snake::sleepTargetDrawPeriod();
	}

L_quit:
	return 0;
}