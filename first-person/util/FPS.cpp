#include "FPS.h"

FPS::FPS()
	: frameCount(0), fps(0)
{
}

const unsigned int FPS::getFPS()
{
	return fps;
}

void FPS::update()
{
	if (clock.getElapsedTime().asSeconds() >= 1.f)
	{
		this->fps = frameCount;
		frameCount = 0;
		clock.restart();
	}

	++frameCount;
}
