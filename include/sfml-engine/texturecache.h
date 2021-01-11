#pragma once 

#include <SFML/Graphics.hpp>

#include <memory>
#include <unordered_map>


namespace gbh 
{
	class TextureCache 
	{
	public:
		static TextureCache& getInstance();
		sf::Texture* getTextureAtPath(const std::string& filename);

	private:
		std::unordered_map<std::string, sf::Texture> m_textures;
	};
}