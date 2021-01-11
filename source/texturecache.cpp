#include "sfml-engine/texturecache.h"


gbh::TextureCache& gbh::TextureCache::getInstance()
{
	static gbh::TextureCache instance;
	return instance;
}


sf::Texture* gbh::TextureCache::getTextureAtPath(const std::string& filename)
{
	if (m_textures.find(filename) == m_textures.end())
	{
		m_textures[filename] = sf::Texture();
	}

	sf::Texture& texture = m_textures[filename];

	if (texture.getNativeHandle() == 0)
	{
		texture.loadFromFile(filename);
	}

	return &texture;
}
