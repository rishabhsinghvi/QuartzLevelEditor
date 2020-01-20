#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "SFML/Graphics.hpp"

#include<unordered_map>
#include<memory>
#include<optional>

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

		std::optional<sf::Texture> getTextureRef(const std::string& textureName) const;

		std::optional<const sf::Texture*> getTexturePointer(const std::string& textureName) const;

		const sf::Texture& getLoadedTextureRef(const std::string& textureName) const;

		const std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& getTextureList() const;

		void renameTexture(const std::string& prev, const std::string& newN);

	};
}

#endif