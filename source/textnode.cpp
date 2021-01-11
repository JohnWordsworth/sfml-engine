#include "sfml-engine/textnode.h"


gbh::TextNode::TextNode(const std::string& text, const sf::Font& font, unsigned int characterSize)
{
	m_text = sf::Text(text, font, characterSize);
}


sf::FloatRect gbh::TextNode::getLocalBounds() const
{
	return m_text.getLocalBounds();
}


void gbh::TextNode::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	target.draw(m_text, transform);
}

