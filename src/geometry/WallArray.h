#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Wall.h"

class WallArray
{
public:
	WallArray();

	void append(const Wall& wall);

	void draw();

private:
	std::vector<Wall> walls;
};
