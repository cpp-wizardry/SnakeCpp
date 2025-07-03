#include "board.h"
#include "snake.h"
#include "fruit.h"

int Board::m_BoardSize = 16;
std::random_device rd;
std::mt19937 gen(rd());

Board::Board()
{
    for (int x = 0; x < m_BoardSize; x++)
    {
        for (int y = 0; y < m_BoardSize; y++)
        {
            int current = x * 16 + y;

            if (current == (m_BoardSize * m_BoardSize) / 2)
            {
                m_Board.push_back(std::make_shared<Snake>(5.0f, 1, 150));
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
    for (int i = 0; i < int(m_Board.size()); i++)
    {
        if (std::dynamic_pointer_cast<Point>(m_Board[i]))
        {
            emptyPositions.push_back(i);
        }
    }

    if (emptyPositions.empty()) return nullptr; 

    std::uniform_int_distribution<> distrib(0, int(emptyPositions.size()) - 1);

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
