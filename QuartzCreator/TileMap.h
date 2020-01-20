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
		unsigned int m_tileSizeX, m_tileSizeY;
		unsigned int m_tileMapWidth, m_tileMapHeight;
		std::string m_textureName;
		std::string m_tileMapFile;
		TextureManager* m_tManager;
		
	public:

		TileMap() = default;

		TileMap(const std::string& tileMapPath, const std::string& texturePath, TextureManager* tManager);

		void RenderMap() const;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		std::string GetTextureName() const;

		void SetTextureName(const std::string& name);

	private:
		void CreateTileMap(); 



	};
}

#endif