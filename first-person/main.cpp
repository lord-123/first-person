#include <SFML/Graphics.hpp>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include "Player.h"

#define WIDTH 800
#define HEIGHT 600

sf::VertexArray vertices = sf::VertexArray::VertexArray();
Player player;

void loadData();
void renderingThread(sf::RenderWindow*);
void handleInput(sf::RenderWindow*);

int main(int argc, char* argv[])
{
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
	vertices.setPrimitiveType(sf::LinesStrip);

	vertices.append(sf::Vertex(sf::Vector2f(193.f, 300.f)));
	vertices.append(sf::Vertex(sf::Vector2f(386.f, 302.f)));
	vertices.append(sf::Vertex(sf::Vector2f(420.f, 224.f)));
	vertices.append(sf::Vertex(sf::Vector2f(510.f, 296.f)));
	vertices.append(sf::Vertex(sf::Vector2f(536.f, 203.f)));

	player = Player(sf::Vector2f(356.f, 94.f), M_PI / 2);
}

void renderingThread(sf::RenderWindow* window)
{
	window->setActive(true);

	sf::Shader shader;
	shader.loadFromFile("shader.vert", "shader.geom", "shader.frag");
	if (!shader.isAvailable())
	{
		std::cout << "shader not available!" << std::endl;
	}

	sf::Font font;
	if (!font.loadFromFile("cnr.otf"))
	{
		std::cout << "failed to load font!" << std::endl;
	}

	sf::Clock clock;
	sf::Text fpsText;
	fpsText.setFont(font);
	fpsText.setCharacterSize(24);
	fpsText.setFillColor(sf::Color::White);

	while (window->isOpen())
	{
		window->clear(sf::Color::Black);

		shader.setUniform("playerPos", player.getPos());
		shader.setUniform("playerFacing", player.getFacing());
		window->draw(vertices, &shader);

		fpsText.setString(std::to_string(1 / clock.restart().asSeconds()));
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
		sf::Time dTime = clock.getElapsedTime();
		float scalar = dTime.asMicroseconds() * 0.0000000001;

		float cosine = cos(player.getFacing());
		float sine = sin(player.getFacing());

		sf::Vector2f pos = player.getPos();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			player.setPos(pos.x + scalar*cosine, pos.y+scalar*sine);
		}
	}
}