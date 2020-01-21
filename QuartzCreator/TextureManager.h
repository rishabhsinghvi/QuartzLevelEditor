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

	public:
		struct TextureInfo
		{
			std::unique_ptr<sf::Texture> m_Texture;
			std::string m_Path;
		};

	private:

		std::unordered_map<std::string, TextureInfo> m_TextureList;

	public:

		void LoadConfigData(const std::vector<Config::TextureRecord>& records);

		void LoadTexture(const std::string& name, const std::string& path);

		std::optional<sf::Texture> GetTextureRef(const std::string& textureName) const;

		std::optional<const sf::Texture*> GetTexturePointer(const std::string& textureName) const;

		const sf::Texture& GetLoadedTextureRef(const std::string& textureName) const;

		const std::unordered_map<std::string, TextureManager::TextureInfo>& GetTextureList() const;

		void RenameTexture(const std::string& prev, const std::string& newN);

		void DeleteTexture(const std::string& name);

	};
}

#endif