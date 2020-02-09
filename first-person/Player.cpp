#include "Player.h"

Player::Player()
{
	pos = sf::Vector2f(0.f, 0.f);
	facing = 0.f;
}

Player::Player(sf::Vector2f pos, float facing)
	: pos(pos), facing(facing)
{
}

sf::Vector2f Player::getPos()
{
	return this->pos;
}

float Player::getFacing()
{
	return this->facing;
}

void Player::setPos(sf::Vector2f pos)
{
	this->pos = pos;
}

void Player::setPos(float x, float y)
{
	this->pos = sf::Vector2f(x, y);
}

void Player::turn(float angle)
{
	this->facing += angle;
}
