#pragma once
#include <SFML\Graphics.hpp>

class Player
{
public:
	Player();
	Player(sf::Vector2f pos, float facing);

	sf::Vector2f getPos();
	const float getFacing();

	void setPos(sf::Vector2f pos);
	void setPos(float x, float y);

	void turn(float angle);

private:
	sf::Vector2f pos;

	float facing;
};