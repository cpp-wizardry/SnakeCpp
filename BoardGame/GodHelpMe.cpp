#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <exception>
#include <Windows.h>
#include <random>
#include <math.h>
#include <string>
#include <conio.h>

class Board;

class Entity
{
public:
	virtual ~Entity() = default;
	virtual void render() const = 0;

	int getPosition() const { return Position; }
	void setPosition(int pos) { Position = pos; }
	virtual bool isCollidable() { return false; };

protected:
	int Position;
};

class Snake : public Entity
{
public:
	Snake(float speed, int score, int startPosition)
		: m_Speed(speed), m_Score(score)
	{
		m_Body.push_back(startPosition);
		Position = startPosition;
	}
	void addSegment(int count)
	{
		for (int i = 0; i < count; ++i)
		{
			m_Body.push_back(m_Body.back()); //rajout d'un segment dans la derniere position
		}
	}


	void addSpeed(float speed);

	void removeSpeed(float speed);

	void move(int direction);

	void render() const override {
		//system("color 0C");
		std::cout << "\033[34m~\033[0m"; //ANSI code color pour changement de la couleur dans terminal
	}

	void setPosition(int pos) { position = pos; };

	int getPosition() const { return m_Body[0]; }

	bool isCollidable() override { return true; };

	void addScore(int score) { this->m_Score += score; };

	int getScore() { return m_Score; };
	std::vector<int> getBody() { return m_Body; };
	int wrap(int value, int max);


private:

	float m_Speed = 5.0f;
	int m_Length = 1;
	int position;
	const int _OFFSET = 16;
	int m_Score = 0;
	int m_currentDirection = 1;


	std::vector<int> m_Body;//liste des segments du serpent
	friend std::ostream& operator<<(std::ostream& os, const Snake& snake);



};

class Fruit : public Entity
{
public:
	Fruit(int pos) { this->Position = pos; }
	bool isCollidable() override { return true; };
	void render() const override {
		std::cout << "%";
	}
	int getPoints() { return points; };

private:
	int points = 1000;

};
namespace Fenetre {

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND CreateMainWindow(Board* board);
	void WNDRenderBoard(HWND hwnd, HDC hdc, const Board& board);
}


class Board
{

public:
	Board();
	void renderBoard();
	static int getBoardSize() { return m_BoardSize; };
	static void setBoardSize(int Size) { m_BoardSize = Size; }
	std::shared_ptr<Snake> getSnake() const;
	void moveSnake(int direction);
	const std::vector<std::shared_ptr<Entity>>& getEntities() const { return m_Board; }

	void setWindowHandle(HWND hwnd) { this->hwnd = hwnd; };
	std::shared_ptr<Fruit> spawnFruit();
	std::shared_ptr<Entity> getEntityAt(int index) const
	{
		return m_Board[index];
	}
	int getSnakeSegmentOrder(int index) const;


private:
	std::vector<std::shared_ptr<Entity>>m_Board;
	static int m_BoardSize;
	HWND hwnd = nullptr;

};

class Point : public Entity
{
public:
	Point(int x, int y);

	int getPosX() { return x; };
	int getPosY() { return y; };
	bool isCollidable() override { return false; };
	void render() const override {
		//system("color 0A");
		std::cout << "\033[32mo\033[0m";
	}

private:
	int x, y;
	int Position = 0;

	friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

void Snake::addSpeed(float speed)
{
    m_Speed += speed;
}
void Snake::removeSpeed(float speed)
{
    m_Speed -= speed;
}
int Snake::wrap(int value, int max) {
    return (value + max) % max;
}

void Snake::move(int direction)
{
    //gros pavé pour check si le joueur fait demi tour direct*
    //[TODO] rendre lisible et optimisé
    if ((m_currentDirection == 1 && direction == 3) ||
        (m_currentDirection == 3 && direction == 1) ||
        (m_currentDirection == 2 && direction == 4) ||
        (m_currentDirection == 4 && direction == 2))
    {
        direction = m_currentDirection; // on ignore si le joueur fait un 180°
    }
    else
    {
        m_currentDirection = direction;
        int head = m_Body.front();
        int row = head / 16;
        int col = head % 16;

        switch (direction)
        {
        case 1: col--; break;
        case 2: row--; break;
        case 3: col++; break;
        case 4: row++; break;
        }

        row = wrap(row, 16);
        col = wrap(col, 16);

        int newHead = row * 16 + col;

        // check la collision on skip la tete sinon probleme
        for (size_t i = 1; i < m_Body.size(); ++i)
        {
            if (newHead == m_Body[i])
            {
                std::cout << "Parti fini" << "\n";
                exit(0); // on quitte pour l'instant 
                //[TODO] faire un menu et un écran de fin
            }
        }

        for (int i = m_Body.size() - 1; i > 0; --i)
        {
            m_Body[i] = m_Body[i - 1];
        }

        m_Body[0] = newHead;
        Position = newHead;
    }


}



std::ostream& operator<<(std::ostream& os, const Snake& snake)
{
    os << "~";
    return os;
}
Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
    this->Position = y * 16 + x;
}
std::ostream& operator<<(std::ostream& os, const Point& point)
{
    //os << "Point(x: " << point.x << ", y: " << point.y << ", place: " << point.place << ")";
    os << 'o';
    return os;
}

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

    HWND CreateMainWindow(Board* board)
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

}int Board::m_BoardSize = 16;


Board::Board()
{
    for (int x = 0; x < m_BoardSize; x++)
    {
        for (int y = 0; y < m_BoardSize; y++)
        {
            int current = x * 16 + y;

            if (current == (m_BoardSize * m_BoardSize) / 2)
            {
                m_Board.push_back(std::make_shared<Snake>(5.0f, 1, 128));
            }
            else
            {
                m_Board.push_back(std::make_shared<Point>(y, x));
            }
        }
    }
}

void Board::renderBoard()
{
    int TotalSize = m_BoardSize * m_BoardSize;
    system("cls"); //clear le terminal à chaque nouveau render

    for (int i = 0; i < TotalSize; i++)
    {
        m_Board[i]->render();

        if ((i + 1) % m_BoardSize == 0)
        {
            std::cout << std::endl;
        }
    }
}


std::shared_ptr<Snake> Board::getSnake() const
{
    for (auto& entity : m_Board)
    {

        std::shared_ptr<Snake> snake = std::dynamic_pointer_cast<Snake>(entity);
        if (snake != nullptr)
        {
            return snake;
        }
    }
    return nullptr;
}

void Board::moveSnake(int direction)
{
    std::shared_ptr<Snake> snake = getSnake();
    if (!snake) return;

    int oldHeadPosition = snake->getPosition();

    std::vector<int> oldBody = snake->getBody();

    snake->move(direction);

    int newHeadPosition = snake->getPosition();

    if (std::dynamic_pointer_cast<Fruit>(m_Board[newHeadPosition]))
    {
        auto fruit = spawnFruit();
        snake->addSegment(1);
        snake->addScore(fruit->getPoints());
    }

    for (int pos : oldBody)
    {
        int oldY = pos / m_BoardSize;
        int oldX = pos % m_BoardSize;
        m_Board[pos] = std::make_shared<Point>(oldX, oldY);
    }

    for (int pos : snake->getBody())
    {
        m_Board[pos] = snake;
    }

    InvalidateRect(hwnd, NULL, TRUE);
}


std::shared_ptr<Fruit> Board::spawnFruit()
{
    std::vector<int> emptyPositions;
    for (int i = 0; i < m_Board.size(); i++)
    {
        if (std::dynamic_pointer_cast<Point>(m_Board[i]))
        {
            emptyPositions.push_back(i);
        }
    }

    if (emptyPositions.empty()) return nullptr;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, emptyPositions.size() - 1);

    int chosenIndex = emptyPositions[distrib(gen)];

    m_Board[chosenIndex] = std::make_shared<Fruit>(chosenIndex);
    return std::dynamic_pointer_cast<Fruit>(m_Board[chosenIndex]);
}
int Board::getSnakeSegmentOrder(int index) const {
    auto snake = getSnake();

    const auto& body = snake->getBody();

    for (size_t i = 0; i < body.size(); ++i) {
        if (body[i] == index)
            return static_cast<int>(i);
    }
    return -1;
}


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
