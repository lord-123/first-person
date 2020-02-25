#include "Wall.h"

Wall::Wall()
	: left(), right(), front(), rear()
{
	vertices = sf::VertexArray(sf::Lines, 2);
}

Wall::Wall(sf::Vector2f* left, sf::Vector2f* right, Region* front, Region* rear)
	: left(left), right(right), front(front), rear(rear)
{
	vertices = sf::VertexArray(sf::Lines, 2);
	vertices[0] = (*this->left);
	vertices[1] = (*this->right);
}

Wall::Wall(sf::Vector2f* left, sf::Vector2f* right, Region* front)
	: left(left), right(right), front(front), rear()
{
	vertices = sf::VertexArray(sf::Lines, 2);
	vertices[0] = (*this->left);
	vertices[1] = (*this->right);
}

void Wall::setLeft(sf::Vector2f* l)
{
	this->left = l;
	vertices[0] = (*left);
}

void Wall::setRight(sf::Vector2f* r)
{
	this->right = r;
	vertices[1] = (*right);
}

void Wall::setFront(Region* f)
{
	this->front = f;
}

void Wall::setRear(Region* r)
{
	this->rear = r;
}

void Wall::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(vertices, states);
}
