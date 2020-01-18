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
	private:
		std::unordered_map<std::string, std::unique_ptr<TileMap>> m_tileMapList;
		TextureManager* m_tManager;
	public:

		void loadConfigData(const std::vector<Config::TileMapRecord>& records);

		void init(TextureManager* tManager);

		void loadTileMap(const std::string& name, const std::string& path, const std::string& texture);

		const TileMap& getTileMapRef(const std::string& tileMapName) const;

		TileMap* getTileMapPointer(const std::string& tileMapName) const;

		const std::unordered_map<std::string, std::unique_ptr<TileMap>>& getTileMapList() const;
	};
}

#endif