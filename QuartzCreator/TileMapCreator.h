#ifndef TILE_MAP_CREATOR_H
#define TILE_MAP_CREATOR_H

#include <string>

#include "TextureManager.h"
#include  "TileMapManager.h"

#include "SFML/Graphics.hpp"

namespace QuartzCreator
{
	class TileMapCreator
	{
	public:
		
		struct CurrentSelection
		{
			sf::Sprite m_Sprite;
			unsigned int currentLayer = 0;
			unsigned int m_tileIndex = -1; // A negative index means that there is no tile present
		};

	private:
		bool m_Running = false;
		std::string m_Name;
		std::string m_TextureName;
		int numTilesX = 10;
		int numTilesY = 10;
		int tileSizeX = 100;
		int tileSizeY = 100;

		bool m_DataAcquired = false;

		CurrentSelection m_Cur;

		TextureManager* m_TexManager;
		TileMapManager* m_TileManager;

	public:

		void Init(TextureManager* tm, TileMapManager* tpm);

		void EnableCreator();

		void DisplayGUI(sf::RenderWindow* window);

	private:

		void AcquireData();

		void CreateTileMap(sf::RenderWindow* window);


	};
}

#endif