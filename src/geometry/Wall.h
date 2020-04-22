#pragma once
#include <SFML/OpenGL.hpp>
#include "Region.h"

class Wall
{
public:
	Wall();
	Wall(sf::Vector2f* left, sf::Vector2f* right, Region* front, Region* rear);
	Wall(sf::Vector2f* left, sf::Vector2f* right, Region* front);

	void setLeft(sf::Vector2f* l);
	void setRight(sf::Vector2f* r);
	void setFront(Region* f);
	void setRear(Region* r);

	void draw();

private:
	sf::VertexArray vertices;

	sf::Vector2f* left;
	sf::Vector2f* right;

	Region* front;
	Region* rear;
};
