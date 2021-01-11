#pragma once

#include <SFML/graphics.hpp>
#include "node.h"


namespace gbh 
{
	class ShapeNode : public Node
	{
	public:
		ShapeNode(const sf::RectangleShape& shape);
		ShapeNode(const sf::CircleShape& shape);

		void setShape(const sf::RectangleShape& shape);
		void setShape(const sf::CircleShape& shape);

		sf::Shape* getShape();

		virtual sf::FloatRect getLocalBounds() const;


	protected:
		virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

	private:
		std::unique_ptr<sf::Shape> m_shape;
	};
} // namespace
