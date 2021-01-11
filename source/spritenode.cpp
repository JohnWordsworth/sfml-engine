#include "sfml-engine/spritenode.h"

#include "sfml-engine/texturecache.h"


gbh::SpriteNode::SpriteNode(const std::string& filename)
{
	sf::Texture* texture = TextureCache::getInstance().getTextureAtPath(filename);

	if (texture != nullptr) 
	{
		m_sprite.setTexture(*texture);
	}
}


gbh::SpriteNode::SpriteNode(const sf::Sprite& sprite) :
	m_sprite(sprite)
{
	
}


sf::FloatRect gbh::SpriteNode::getLocalBounds() const
{
	return m_sprite.getLocalBounds();
}


void gbh::SpriteNode::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	target.draw(m_sprite, transform);
}

