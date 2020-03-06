#define _USE_MATH_DEFINES

#include <geometry/Sector.cpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "entities/Player.cpp"
#include "util/FPS.cpp"
#include <math.h>
#include <sstream>
#include <helpers/split.cpp>

#ifdef linux
#include <X11/Xlib.h>
#endif

#define WIDTH 800
#define HEIGHT 600

bool debug = false;

std::vector<Sector> sectors;
Player player;

void loadData();
void renderingThread(sf::RenderWindow*);
void handleInput();

int main(int argc, char* argv[])
{
#ifdef linux
	XInitThreads();
#endif

	loadData();
	sf::ContextSettings settings;

	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "3D engine", sf::Style::Close | sf::Style::Resize, settings);


	sf::Thread renderer(&renderingThread, &window);
	window.setActive(false);
	renderer.launch();

	sf::Thread input(&handleInput);
	input.launch();

	while (true)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				renderer.terminate();
				input.terminate();
				return 0;
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
	input.open("resources/levels/newlvl.txt");

	std::string line;

	
	std::vector<sf::Vector2f> vertices;

	while (std::getline(input, line))
	{
		std::stringstream ss(line);

		std::vector<std::string> partitions = split(ss, ' ');

		if (!partitions.size()) continue;

		else if (partitions[0] == "#")
		{
			continue;
		}

		else if (partitions[0] == "vertex")
		{
			sf::Vector2f vertex;

			vertex.x = static_cast<float>(std::stoi(partitions[1]));
			vertex.y = static_cast<float>(std::stoi(partitions[2]));

			vertices.push_back(vertex);
		}

		else if (partitions[0] == "sector")
		{
			Sector sector;

			sector.ceiling = std::stoi(partitions[1]);
			sector.floor = std::stoi(partitions[2]);

			int length = (partitions.size() - 3) / 2;

			for (int i = 0; i < length; i++)
			{
				sector.addVertex(&vertices[std::stoi(partitions[i + 3])]);
			}

			for (int i = length + 3; i < partitions.size(); i++)
			{
				sector.neighbours.push_back(std::stoi(partitions[i]));
			}

			sectors.push_back(sector);
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
	std::cout << "shaders available: " << sf::Shader::isAvailable() << std::endl;
	std::cout << "geometry shader available: " << sf::Shader::isGeometryAvailable() << std::endl;

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

	sf::Texture texture;
	if (!texture.loadFromFile("resources/textures/minecraft_stone.png"))
	{
		std::cout << "failed to load texture!" << std::endl;
	}

	texture.setRepeated(true);

	shader.setUniform("FOV", (float)M_PI / 2);
	shader.setUniform("img", texture);

	while (true)
	{
		fps.update();

		window->clear(sf::Color::Black);

		shader.setUniform("playerPos", player.getPos());
		shader.setUniform("playerFacing", player.getFacing());
		for (int i = 0; i < sectors.size(); i++)
		{
			window->draw(sectors[i], &shader);
		}

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

void handleInput()
{
	sf::Clock clock;
	int lastMouseX = sf::Mouse::getPosition().x;

	while (true)
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
