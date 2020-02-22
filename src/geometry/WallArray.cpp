#include "WallArray.h"

WallArray::WallArray()
	: walls()
{
}

void WallArray::append(const Wall& wall)
{
	walls.push_back(wall);
}

void WallArray::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < walls.size(); i++)
	{
		target.draw(walls[i], states);
	}
}
