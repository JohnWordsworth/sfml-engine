#pragma once

#include <SFML/graphics.hpp>

#include "node.h"


namespace gbh {

	class SpriteNode : public Node
	{
	public:
		SpriteNode(const std::string& filename);
		SpriteNode(const sf::Sprite& sprite);
        virtual ~SpriteNode() { };

        void setSprite(const sf::Sprite& sprite);

        void setColor(const sf::Color& color);
        const sf::Color& getColor() const;
        
		virtual sf::FloatRect getLocalBounds() const;

    protected:
		virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;
	
	private:
		sf::Sprite m_sprite;
	};

} // namespace
