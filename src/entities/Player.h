#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
	Player();
	Player(sf::Vector2f pos, float facing, int sector);

	const sf::Vector2f getPos();
	const float getFacing();
	const int getSector();

	void setPos(sf::Vector2f pos);
	void setPos(float x, float y);

	void turn(float angle);

private:
	sf::Vector2f pos;
	int sector;

	float facing;
};
