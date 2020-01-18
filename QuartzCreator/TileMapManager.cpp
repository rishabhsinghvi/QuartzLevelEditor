#include "TileMap.h"
#include "TileMapManager.h"

#include<iostream>

namespace QuartzCreator
{
	void TileMapManager::init(TextureManager* tManager)
	{
		m_tManager = tManager;
	}
	void TileMapManager::loadTileMap(const std::string& name, const std::string& path, const std::string& texture)
	{ 
		auto tileMap = std::make_unique<TileMap>(path, texture, m_tManager);

		m_tileMapList[name] = std::move(tileMap);

	}
	const TileMap& TileMapManager::getTileMapRef(const std::string& tileMapName) const
	{
		auto found = m_tileMapList.find(tileMapName);
		if (found == m_tileMapList.end())
		{
			std::cout << "Cannot find tilemap: " << tileMapName << '\n';
			__debugbreak();
		}

		return *(found->second);
	}
	TileMap* TileMapManager::getTileMapPointer(const std::string& tileMapName) const
	{
		auto found = m_tileMapList.find(tileMapName);
		if (found == m_tileMapList.end())
		{
			std::cout << "Cannot find tilemap: " << tileMapName << '\n';
			__debugbreak();
		}

		return found->second.get();
	}
	const std::unordered_map<std::string, std::unique_ptr<TileMap>>& TileMapManager::getTileMapList() const
	{
		return m_tileMapList;
	}
}