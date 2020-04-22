#include "WallArray.h"

WallArray::WallArray()
	: walls()
{
}

void WallArray::append(const Wall& wall)
{
	walls.push_back(wall);
}

void WallArray::draw()
{
	for (int i = 0; i < walls.size(); i++)
	{
		walls[i].draw();
	}
}
