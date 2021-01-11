#pragma once

#include "node.h"

namespace gbh
{
	class TextNode : public Node
	{
	public:
		TextNode(const std::string& text, const sf::Font& font, unsigned int characterSize = 30);

		virtual sf::FloatRect getLocalBounds() const;

	protected:
		virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

	private:
		sf::Text m_text;
	};
}