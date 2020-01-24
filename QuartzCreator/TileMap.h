#ifndef TILEMAP_LOADER_H
#define TILEMAP_LOADER_H

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics//Transformable.hpp"
#include "SFML/Graphics/VertexArray.hpp"

#include<vector>
#include<string>

#include "TextureManager.h"

namespace QuartzCreator
{

	class TileMap: public sf::Drawable, public sf::Transformable
	{
	private:
		std::vector<sf::VertexArray> m_Vertices;
		int m_tileSizeX, m_tileSizeY;
		unsigned int m_tileMapWidth, m_tileMapHeight;
		std::string m_textureName;
		std::string m_tileMapFile;
		TextureManager* m_tManager;
		
	public:

		TileMap() = default;

		TileMap(TextureManager* tm);

		TileMap(const std::string& tileMapPath, const std::string& texturePath, TextureManager* tManager);

		void RenderMap() const;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		std::string GetTextureName() const;

		void SetTextureName(const std::string& name);

		void AddLayer(const std::vector<int>& layer);

		void CreateNewLayer();

		void DeleteAllLayers();

		void DeleteLayer(unsigned int layer);

		unsigned int GetNumLayers() const;

		void SetTileSize(unsigned int tileSizex, unsigned int tileSizeY);

		void SetTileMapDimensions(unsigned int width, unsigned int height);

		void AddTileAt(unsigned int layer, unsigned int i, unsigned int j, unsigned int index);

		void Clear();

		void FillLayer(unsigned int layer, int tileIndex);

	private:
		void CreateTileMap(); 



	};
}

#endif