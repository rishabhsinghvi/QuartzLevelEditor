#include "TextureManager.h"

#include<iostream>


namespace QuartzCreator
{
	class TextureManager;

	void TextureManager::loadTexture(const std::string& name, const std::string& path)
	{
		auto texture = std::make_unique<sf::Texture>();
		if (!texture->loadFromFile(path))
		{
			std::cout << "Unable to load texture from:  " << path << '\n';
			__debugbreak();
		}

		m_TextureList[name] = std::move(texture);
		std::cout << "TextureManager: Succesfully created texture: " << name << '\n';
	}

	const sf::Texture& TextureManager::getTextureRef(const std::string& textureName) const
	{
		auto found = m_TextureList.find(textureName);

		if (found == m_TextureList.end())
		{
			std::cout << "Texture not available\n";
			__debugbreak();
		}

		return *(found->second);
	}

	const sf::Texture* QuartzCreator::TextureManager::getTexturePointer(const std::string& textureName) const
	{
		auto found = m_TextureList.find(textureName);

		if (found == m_TextureList.end())
		{
			std::cout << "Texture not available\n";
			__debugbreak();
		}

		return found->second.get();
	}

	const std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& TextureManager::getTextureList() const
	{
		return m_TextureList;
	}

}