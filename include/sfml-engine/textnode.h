#pragma once

#include "node.h"

namespace gbh
{
	class TextNode : public Node
	{
	public:
		TextNode(const std::string& text, const sf::Font& font, unsigned int characterSize = 30);

        void setString(const sf::String& value);
        const sf::String& getString() const { return m_text.getString(); }
        
        void setCharacterSize(int size);
        int getCharacterSize() const { return m_text.getCharacterSize(); }
        
        void setFillColor(const sf::Color& color);
        const sf::Color& getFillColor() const { return m_text.getFillColor(); }
        
        void setOutlineColor(const sf::Color& color);
        const sf::Color& getOutlineColor() const { return m_text.getOutlineColor(); }
        
        void setOutlineThickness(float thickness);
        const float getOutlineThickness() const { return m_text.getOutlineThickness(); }
        
		virtual sf::FloatRect getLocalBounds() const;

	protected:
		virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

	private:
		sf::Text m_text;
	};
}
