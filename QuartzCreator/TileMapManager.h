#ifndef TILEMAP_MANAGER_H
#define TILEMAP_MANAGER_H

#include <unordered_map>
#include <string>
#include <memory>

#include "TileMap.h"
#include "TextureManager.h"
#include "Config.h"

namespace QuartzCreator
{
	extern const unsigned int WINDOW_WIDTH;
	extern const unsigned int WINDOW_HEIGHT;

	class TileMapManager
	{
	public:
		struct TileMapInfo
		{
			std::unique_ptr<TileMap> m_TileMap;
			std::string m_Path;
			std::string m_TextureName;
		};

	private:
		std::unordered_map<std::string, TileMapInfo> m_tileMapList;
		TextureManager* m_tManager;
	public:

		void LoadConfigData(const std::vector<Config::TileMapRecord>& records);

		void Init(TextureManager* tManager);

		void LoadTileMap(const std::string& name, const std::string& path, const std::string& texture);

		const TileMap& GetTileMapRef(const std::string& tileMapName) const;

		TileMap* GetTileMapPointer(const std::string& tileMapName) const;

		const std::unordered_map<std::string, TileMapInfo>& GetTileMapList() const;

		void ChangeTextureName(std::string& name, std::string& newVal);
	};
}

#endif