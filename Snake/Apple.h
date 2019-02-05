#pragma once
#include <vector>

struct ApplePart
{
	double x, y;

	ApplePart(double x, double y) : x(x), y(y)
	{
	}
};


class Apple
{
public:
	Apple(double x, double y);

	unsigned int getSizeWidth() const;
	unsigned int getSizeHeight() const;
	double getX() const;
	double getY() const;
	const ApplePart& getApplePart(unsigned int i) const;

	void setSizeWidth(unsigned int width);
	void setSizeHeight(unsigned int height);
	void setX(double x);
	void setY(double y);

	~Apple();

private:
	unsigned int sizeWidth;
	unsigned int sizeHeight;
	double x;
	double y;

	std::vector<ApplePart>  appleParts;
};


Apple::Apple(double x, double y) : x(x), y(y)
{
	this->sizeHeight = 6.666;
	this->sizeWidth = 6.666;

	this->appleParts.push_back(ApplePart(0, this->sizeHeight));
	this->appleParts.push_back(ApplePart(this->sizeWidth, 2 * this->sizeHeight));
	this->appleParts.push_back(ApplePart(2 * this->sizeWidth, this->sizeHeight));
	this->appleParts.push_back(ApplePart(this->sizeWidth, 0));
}

inline unsigned int Apple::getSizeWidth() const
{
	return this->sizeWidth;
}

inline unsigned int Apple::getSizeHeight() const
{
	return this->sizeHeight;
}

inline double Apple::getX() const
{
	return this->x;
}

inline double Apple::getY() const
{
	return this->y;
}

inline const ApplePart & Apple::getApplePart(unsigned int i) const
{
	return this->appleParts[i];
}

inline void Apple::setSizeWidth(unsigned int width)
{
	this->sizeWidth = width;
}

inline void Apple::setSizeHeight(unsigned int height)
{
	this->sizeHeight = height;
}

inline void Apple::setX(double x)
{
	this->x = x;
}

inline void Apple::setY(double y)
{
	this->y = y;
}

Apple::~Apple()
{
}
