#include "sfml-engine/spritenode.h"

#include "sfml-engine/texturecache.h"


gbh::SpriteNode::SpriteNode(const std::string& filename)
{
	sf::Texture* texture = TextureCache::getInstance().getTextureAtPath(filename);
    
	if (texture != nullptr) 
	{
        setSprite(sf::Sprite(*texture));
	}
}


gbh::SpriteNode::SpriteNode(const sf::Sprite& sprite) :
	m_sprite(sprite)
{
	
}


void gbh::SpriteNode::setSprite(const sf::Sprite &sprite)
{
    m_sprite = sprite;
    updateAbsoluteOrigin();
}


void gbh::SpriteNode::setColor(const sf::Color& color)
{
    m_sprite.setColor(color);
}


const sf::Color& gbh::SpriteNode::getColor() const
{
    return m_sprite.getColor();
}


sf::FloatRect gbh::SpriteNode::getLocalBounds() const
{
	return m_sprite.getLocalBounds();
}


void gbh::SpriteNode::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	target.draw(m_sprite, transform);
}

