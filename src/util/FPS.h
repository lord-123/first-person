#pragma once
#include <SFML\System\Clock.hpp>
//inspired by https://gist.github.com/kenpower/7233967

class FPS
{
public:
	FPS();

	const unsigned int getFPS();

	void update();

private:
	unsigned int frameCount;
	unsigned int fps;
	sf::Clock clock;
};

