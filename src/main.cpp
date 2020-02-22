#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "entities/Player.cpp"
#include "util/FPS.cpp"
#include "geometry/Region.h"
#include "geometry/Wall.cpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <geometry\WallArray.cpp>

#define WIDTH 800
#define HEIGHT 600

WallArray walls;
Player player;

void loadData();
void renderingThread(sf::RenderWindow*);
void handleInput(sf::RenderWindow*);

int main(int argc, char* argv[])
{
	std::cout << argv[0] << std::endl;

	loadData();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "3D engine", sf::Style::Close | sf::Style::Resize, settings);
	window.setActive(false);

	sf::Thread renderer(&renderingThread, &window);
	renderer.launch();

	sf::Thread input(&handleInput, &window);
	input.launch();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
	}

	return 0;
}

void loadData()
{
	//TODO: read in from file bloody hell this is bad
	Region regions[2];
	regions[0].ceilingY = 5;
	regions[0].floorY = -5;
	regions[1].ceilingY = 5;
	regions[1].floorY = -5;

	sf::Vector2f vertices[8];
	vertices[0].x = 0;
	vertices[0].y = 0;

	vertices[1].x = 0;
	vertices[1].y = 500;

	vertices[2].x = 500;
	vertices[2].y = 500;

	vertices[3].x = 500;
	vertices[3].y = 0;


	vertices[4].x = 200;
	vertices[4].y = 200;

	vertices[5].x = 200;
	vertices[5].y = 300;

	vertices[6].x = 300;
	vertices[6].y = 300;

	vertices[7].x = 300;
	vertices[7].y = 200;

	walls.append(Wall(&vertices[0], &vertices[1], &regions[0]));
	walls.append(Wall(&vertices[1], &vertices[2], &regions[0]));
	walls.append(Wall(&vertices[2], &vertices[3], &regions[0]));
	walls.append(Wall(&vertices[3], &vertices[0], &regions[0]));

	walls.append(Wall(&vertices[4], &vertices[5], &regions[1], &regions[0]));
	walls.append(Wall(&vertices[5], &vertices[6], &regions[1], &regions[0]));
	walls.append(Wall(&vertices[6], &vertices[7], &regions[1], &regions[0]));
	walls.append(Wall(&vertices[7], &vertices[4], &regions[1], &regions[0]));

	player = Player(sf::Vector2f(356.f, 94.f), M_PI / 2);
}

void renderingThread(sf::RenderWindow* window)
{
	window->setActive(true);

	sf::Shader shader;
	shader.loadFromFile("shaders/shader.vert", "shaders/shader.geom", "shaders/shader.frag");
	if (!shader.isAvailable())
	{
		std::cout << "shader not available!" << std::endl;
	}

	sf::Font font;
	if (!font.loadFromFile("resources/fonts/cnr.otf"))
	{
		std::cout << "failed to load font!" << std::endl;
	}

	sf::Text fpsText;
	fpsText.setFont(font);
	fpsText.setCharacterSize(24);
	fpsText.setFillColor(sf::Color::Green);

	FPS fps;

	while (window->isOpen())
	{
		fps.update();

		window->clear(sf::Color::Black);

		shader.setUniform("playerPos", player.getPos());
		shader.setUniform("playerFacing", player.getFacing());
		window->draw(walls, &shader);

		fpsText.setString("fps: " + std::to_string(fps.getFPS()));
		window->draw(fpsText);

		window->display();
	}
}

void handleInput(sf::RenderWindow* window)
{
	sf::Clock clock;
	int lastMouseX = sf::Mouse::getPosition().x;

	while (window->isOpen())
	{
		sf::Time dTime = clock.restart();
		float scalar = dTime.asMicroseconds()/200;

		float cosine = cos(player.getFacing());
		float sine = sin(player.getFacing());

		sf::Vector2f pos = player.getPos();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			player.setPos(pos.x + scalar*cosine, pos.y+scalar*sine);
		}

		int newMouseX = sf::Mouse::getPosition().x;
		player.turn(-0.01f * (newMouseX - lastMouseX));
		lastMouseX = newMouseX;
	}
}