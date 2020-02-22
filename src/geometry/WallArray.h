#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "Wall.h"

class WallArray : public sf::Drawable
{
public:
	WallArray();

	void append(const Wall& wall);

private:
	std::vector<Wall> walls;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};