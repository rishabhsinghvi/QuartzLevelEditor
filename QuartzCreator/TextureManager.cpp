#include "TextureManager.h"

#include<iostream>

namespace QuartzCreator
{
	class TextureManager;

	void TextureManager::LoadConfigData(const std::vector<Config::TextureRecord>& records)
	{
		for (const auto& record : records)
		{
			LoadTexture(record.m_Name, record.m_PathToFile);
		}
	}

	void TextureManager::LoadTexture(const std::string& name, const std::string& path)
	{
		auto texture = std::make_unique<sf::Texture>();
		if (!texture->loadFromFile(path))
		{
			std::cout << "Unable to load texture from:  " << path << '\n';
			return;
		}
		TextureInfo info;
		info.m_Path = path;
		info.m_Texture = std::move(texture);

		m_TextureList[name] = std::move(info);
		std::cout << "TextureManager: Succesfully created texture: " << name << '\n';
	}

	std::optional<sf::Texture> TextureManager::GetTextureRef(const std::string& textureName) const
	{
		auto found = m_TextureList.find(textureName);

		if (found == m_TextureList.end())
		{
			std::cout << "Texture not available: " << textureName <<"\n";
			return {};
		}

		return *(found->second.m_Texture);
	}

	std::optional<const sf::Texture*> QuartzCreator::TextureManager::GetTexturePointer(const std::string& textureName) const
	{
		auto found = m_TextureList.find(textureName);

		if (found == m_TextureList.end())
		{
			std::cout << "Texture not available: " << textureName << '\n';
			return {};
		}

		return found->second.m_Texture.get();
	}

	// GetLoadedTextureRef is the same as GetTextureRef, but it assumes that the requested texture WILL be present in the resource manager
	// It does no checking whatsoever. Use GetTextureRef() as much as possible

	const sf::Texture& TextureManager::GetLoadedTextureRef(const std::string& textureName) const
	{
		return *m_TextureList.find(textureName)->second.m_Texture;
	}

	const std::unordered_map<std::string, TextureManager::TextureInfo>& TextureManager::GetTextureList() const
	{
		return m_TextureList;
	}

	void TextureManager::RenameTexture(const std::string& prev, const std::string& newName)
	{
		auto found = m_TextureList.find(prev);

		if (found == m_TextureList.end())
			return;

		auto val = std::move(found->second);

		m_TextureList.erase(found);

		m_TextureList[newName] = std::move(val);

		std::cout << "Renamed texture from " << prev << " to " << newName << '\n';
	}

	void TextureManager::DeleteTexture(const std::string& name)
	{
		auto found = m_TextureList.find(name);

		if (found == m_TextureList.end())
			return;

		m_TextureList.erase(found);
	}

}