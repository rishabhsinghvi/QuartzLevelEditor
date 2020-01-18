#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "SFML/Graphics.hpp"

#include<unordered_map>
#include<memory>

#include "Config.h"

namespace QuartzCreator
{
	class TextureManager
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_TextureList;

	public:

		void loadConfigData(const std::vector<Config::TextureRecord>& records);

		void loadTexture(const std::string& name, const std::string& path);

		const sf::Texture& getTextureRef(const std::string& textureName) const;

		const sf::Texture* getTexturePointer(const std::string& textureName) const;

		const std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& getTextureList() const;

	};
}

#endif