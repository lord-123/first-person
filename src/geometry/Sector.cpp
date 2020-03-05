#include "Sector.h"

Sector::Sector()
{
	vertices = sf::VertexArray(sf::PrimitiveType::LineStrip);
}

void Sector::addVertex(sf::Vector2f* vertex)
{
	vertices.append(sf::Vertex(*vertex));
}

void Sector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(vertices, states);
}