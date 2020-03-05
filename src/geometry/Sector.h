#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Sector : public sf::Drawable
{
public:
	Sector();

	float floor;
	float ceiling;

	void addVertex(sf::Vector2f* vertex);
	std::vector<int> neighbours;

private:
	sf::VertexArray vertices;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};