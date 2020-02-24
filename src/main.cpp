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
#include <helpers\split.cpp>

#define WIDTH 800
#define HEIGHT 600

bool debug = false;

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
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::F3) debug = !debug;
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
		std::stringstream ss(line);

		std::vector<std::string> partitions = split(ss, ' ');

		if (!partitions.size()) continue;

		else if (partitions[0] == "region")
		{
			Region region;

			region.ceilingY = std::stoi(partitions[1]);
			region.floorY = std::stoi(partitions[2]);

			regions.push_back(region);
		}

		else if (partitions[0] == "vertex")
		{
			sf::Vector2f vertex;

			vertex.x = static_cast<float>(std::stoi(partitions[1]));
			vertex.y = static_cast<float>(std::stoi(partitions[2]));

			vertices.push_back(vertex);
		}

		else if (partitions[0] == "wall")
		{
			Wall wall(&vertices[std::stoi(partitions[1])], &vertices[std::stoi(partitions[2])], &regions[std::stoi(partitions[3])]);

			if (partitions.size() > 4) wall.setRear(&regions[std::stoi(partitions[4])]);

			walls.append(wall);
		}

		else if (partitions[0] == "player")
		{
			player.setPos(std::stoi(partitions[1]), std::stoi(partitions[2]));
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

		if (debug)
		{
			fpsText.setString("fps: " + std::to_string(fps.getFPS()) + "\n"
							+ "x: " + std::to_string(player.getPos().x) + "\n"
							+ "y: " + std::to_string(player.getPos().y) + "\n"
							+ "facing: " + std::to_string(player.getFacing()) + " (" + std::to_string(player.getFacing() * 180.0 / M_PI) + ")");
			window->draw(fpsText);
		}

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