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
	std::ifstream input;
	input.open("resources/levels/level.txt");

	std::string line;

	std::vector<Region> regions;
	std::vector<sf::Vector2f> vertices;

	while (std::getline(input, line))
	{
		std::string partition;

		std::stringstream ss(line);

		if (!std::getline(ss, partition, ' ')) continue;

		else if (partition == "region")
		{
			Region region;

			std::getline(ss, partition, ' ');
			region.ceilingY = atoi(partition.c_str());
			std::getline(ss, partition, ' ');
			region.floorY = atoi(partition.c_str());

			regions.push_back(region);
		}

		else if (partition == "vertex")
		{
			sf::Vector2f vertex;

			std::getline(ss, partition, ' ');
			vertex.x = atoi(partition.c_str());
			std::getline(ss, partition, ' ');
			vertex.y = atoi(partition.c_str());

			vertices.push_back(vertex);
		}

		else if (partition == "wall")
		{
			Wall wall;

			std::getline(ss, partition, ' ');
			wall.setLeft(&vertices[atoi(partition.c_str())]);
			std::getline(ss, partition, ' ');
			wall.setRight(&vertices[atoi(partition.c_str())]);
			std::getline(ss, partition, ' ');
			wall.setFront(&regions[atoi(partition.c_str())]);

			if (std::getline(ss, partition, ' ')) wall.setRear(&regions[atoi(partition.c_str())]);

			walls.append(wall);
		}

		else if (partition == "player")
		{
			std::string x;
			std::string y;

			std::getline(ss, x, ' ');
			std::getline(ss, y, ' ');

			player.setPos(atoi(x.c_str()), atoi(y.c_str()));
		}
	}

	input.close();
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