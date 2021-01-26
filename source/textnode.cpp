#include "sfml-engine/textnode.h"


gbh::TextNode::TextNode(const std::string& text, const sf::Font& font, unsigned int characterSize)
{
	m_text = sf::Text(text, font, characterSize);
}


void gbh::TextNode::setString(const sf::String& value)
{
    m_text.setString(value);
    updateAbsoluteOrigin();
}


void gbh::TextNode::setCharacterSize(int size)
{
    m_text.setCharacterSize(size);
    updateAbsoluteOrigin();
}


void gbh::TextNode::setFillColor(const sf::Color& color)
{
    m_text.setFillColor(color);
}


void gbh::TextNode::setOutlineColor(const sf::Color& color)
{
    m_text.setOutlineColor(color);
}


void gbh::TextNode::setOutlineThickness(float thickness)
{
    m_text.setOutlineThickness(thickness);
    updateAbsoluteOrigin();
}


sf::FloatRect gbh::TextNode::getLocalBounds() const
{
	return m_text.getLocalBounds();
}


void gbh::TextNode::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	target.draw(m_text, transform);
}

