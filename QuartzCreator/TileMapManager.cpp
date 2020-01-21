#include "TileMap.h"
#include "TileMapManager.h"

#include<iostream>

namespace QuartzCreator
{
	void TileMapManager::LoadConfigData(const std::vector<Config::TileMapRecord>& records)
	{
		for (const auto& record : records)
		{
			LoadTileMap(record.m_Name, record.m_PathToFile, record.m_TextureName);
		}
	}
	void TileMapManager::Init(TextureManager* tManager)
	{
		m_tManager = tManager;
	}
	void TileMapManager::LoadTileMap(const std::string& name, const std::string& path, const std::string& texture)
	{ 
		auto tileMap = std::make_unique<TileMap>(path, texture, m_tManager);
		TileMapInfo tInfo;
		tInfo.m_Path = path;
		tInfo.m_TextureName = texture;
		tInfo.m_TileMap = std::move(tileMap);

		m_tileMapList[name] = std::move(tInfo);

	}
	const TileMap& TileMapManager::GetTileMapRef(const std::string& tileMapName) const
	{
		auto found = m_tileMapList.find(tileMapName);
		if (found == m_tileMapList.end())
		{
			std::cout << "Cannot find tilemap: " << tileMapName << '\n';
			__debugbreak();
		}

		return *(found->second.m_TileMap);
	}
	TileMap* TileMapManager::GetTileMapPointer(const std::string& tileMapName) const
	{
		auto found = m_tileMapList.find(tileMapName);
		if (found == m_tileMapList.end())
		{
			std::cout << "Cannot find tilemap: " << tileMapName << '\n';
			__debugbreak();
		}

		return found->second.m_TileMap.get();
	}
	const std::unordered_map<std::string, TileMapManager::TileMapInfo>& TileMapManager::GetTileMapList() const
	{
		return m_tileMapList;
	}

	void TileMapManager::ChangeTextureName(std::string& prev, std::string& newVal)
	{
		for (auto& map : m_tileMapList)
		{
			if (map.second.m_TextureName == prev)
			{
				map.second.m_TextureName = newVal;
				map.second.m_TileMap->SetTextureName(newVal);
			}
		}
	}
}