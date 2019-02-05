#pragma once
#include <vector>

struct SnakePart
{
	double x, y;
};


class Snake
{
public:
	Snake(double x, double y, double width, double height);

	unsigned int getSize() const;
	double getX() const;
	double getY() const;
	double getWidth() const;
	double getHeight() const;
	int getCurrentDirection() const;

	const static enum {
		Up = 1, Right = 2, Down = 3, Left = 4
	};

	SnakePart &getSnakePart(int index);

	void snakeMoveUP();
	void snakeMoveDOWN();
	void snakeMoveRight();
	void snakeMoveLeft();
	void snakeMoveCurrent();

	void reset(double x, double y);

	Snake& operator++(int);


	virtual ~Snake();

	short red, green, blue;
protected:
	void snakeMove(int direction);

private:
	std::vector<SnakePart> snakeParts;
	SnakePart tail;

	double x, y;
	double width, height;
	unsigned int size;
	double defaultSpeed;
	double maxSpeed;

	double currentSpeed;
	int currentDirection;
};


inline Snake::Snake(double x, double y, double width, double height) : x(x), y(y), width(width), height(height), red(0), green(0), blue(255)
{
	SnakePart temp;
	temp.x = this->x;
	temp.y = this->y;
	this->size = 1;
	this->currentDirection = 1;
	this->snakeParts.push_back(temp);
	tail = temp;
}

inline unsigned int Snake::getSize() const
{
	return this->size;
}

inline double Snake::getX() const
{
	return this->x;
}

inline double Snake::getY() const
{
	return this->y;
}

inline double Snake::getWidth() const
{
	return this->width;
}

inline double Snake::getHeight() const
{
	return this->height;
}

inline int Snake::getCurrentDirection() const
{
	return this->currentDirection;
}

inline SnakePart& Snake::getSnakePart(int index)
{
	return this->snakeParts[index];
}

inline void Snake::snakeMoveUP()
{
	this->currentDirection = Snake::Up;
}

inline void Snake::snakeMoveDOWN()
{
	this->currentDirection = Snake::Down;
}

inline void Snake::snakeMoveRight()
{
	this->currentDirection = Snake::Right;
}

inline void Snake::snakeMoveLeft()
{
	this->currentDirection = Snake::Left;
}

inline void Snake::snakeMoveCurrent()
{
	this->snakeMove(5);
}

inline void Snake::reset(double x, double y)
{
	this->snakeParts.clear();

	Snake temp(x, y, 20, 20);
	*this = temp;

	(*this)++;
	(*this)++;
	this->snakeMoveCurrent();
	this->snakeMoveCurrent();
}

inline Snake & Snake::operator++(int)
{
	if (this->size > 1) {
		snakeParts.push_back(tail);
		this->size++;
	}
	else {
		this->snakeMoveCurrent();
		snakeParts.push_back(tail);
		this->size++;
	}
	return *this;
}


Snake::~Snake()
{
}

inline void Snake::snakeMove(int direction)
{
	// 1 - UP
	// 2 - Right
	// 3 - Down
	// 4 - Left
	// 5 - Current direction
	if (direction != 5) {
		this->currentDirection = direction;
	}

	for (int i = this->size - 1; i > 0; i--) {
		if (i == this->size - 1) {
			this->tail = this->snakeParts[i];
		}
		this->snakeParts[i] = this->snakeParts[i - 1];
		this->snakeParts[i].x -= 0;
	}

	if (this->currentDirection == Snake::Up) {
		this->snakeParts[0].y -= 20;
	}
	else if (this->currentDirection == Snake::Right) {
		this->snakeParts[0].x += 20;
	}
	else if (this->currentDirection == Snake::Down) {
		this->snakeParts[0].y += 20;
	}
	else if (this->currentDirection == Snake::Left) {
		this->snakeParts[0].x -= 20;
	}
}
