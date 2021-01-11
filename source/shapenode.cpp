#include "sfml-engine/shapenode.h"


gbh::ShapeNode::ShapeNode(const sf::RectangleShape& shape)
{
	setShape(shape);
}


gbh::ShapeNode::ShapeNode(const sf::CircleShape& shape)
{
	setShape(shape);
}


void gbh::ShapeNode::setShape(const sf::RectangleShape& shape)
{
	// Copy the shape that we were given so that we don't have to worry about the original
	// being deleted.
	m_shape = std::make_unique<sf::RectangleShape>(shape);
}


void gbh::ShapeNode::setShape(const sf::CircleShape& shape)
{
	// Copy the shape that we were given so that we don't have to worry about the original
	// being deleted.
	m_shape = std::make_unique<sf::CircleShape>(shape);
}


sf::Shape* gbh::ShapeNode::getShape()
{
	return m_shape.get();
}


sf::FloatRect gbh::ShapeNode::getLocalBounds() const
{
	return m_shape->getLocalBounds();
}


void gbh::ShapeNode::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	if (m_shape)
	{
		target.draw(*m_shape, transform);
	}
}