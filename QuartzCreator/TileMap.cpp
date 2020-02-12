#include "TileMap.h"
#include "TextureManager.h"

#include "json.hpp"

#include<fstream>

#include<iostream>


namespace QuartzCreator
{
	TileMap::TileMap(TextureManager* tm)
	{
		m_tManager = tm;
	}
	TileMap::TileMap(const std::string& tileMapLoader, const std::string& textureName, TextureManager* tManager)
		: m_tileMapFile(tileMapLoader), m_textureName(textureName), m_tManager(tManager)
	{
		CreateTileMap();
	}

	void TileMap::RenderMap() const
	{
	}

	void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		auto txPtr = m_tManager->GetTexturePointer(m_textureName);

		if (!txPtr.has_value())
			return;

		states.transform *= getTransform();

		states.texture = txPtr.value();


		for (const auto& x : m_Vertices)
		{
			target.draw(x, states);
		}
	}

	std::string TileMap::GetTextureName() const
	{
		return m_textureName;
	}

	void TileMap::SetTextureName(const std::string& name)
	{
		m_textureName = name;
	}

	void TileMap::AddLayer(const std::vector<int>& layer)
	{
		if (layer.size() != m_tileMapWidth * m_tileMapHeight)
		{
			std::cout << "Tilemap Dimensions don't match! Not adding layer to tilemap!\n";
			return;
		}

		sf::VertexArray vertices;
		vertices.setPrimitiveType(sf::Quads);
		vertices.resize(m_tileMapWidth * m_tileMapHeight * 4);

		auto texturePtr = m_tManager->GetTexturePointer(m_textureName);

		for (auto i = 0; i < m_tileMapWidth; i++)
		{
			for (auto j = 0; j < m_tileMapHeight; j++)
			{
				int tileNumber = layer[i + j * m_tileMapWidth] - 1;

				if (tileNumber < 0)
					continue;

				/*int tu = tileNumber % ((texturePtr.value()->getSize().x / m_tileSizeX));
				int tv = tileNumber / (((texturePtr.value()->getSize().y / m_tileSizeY) - 1));*/


				auto x = (texturePtr.value()->getSize().x / m_tileSizeX);
				auto y = (texturePtr.value()->getSize().y / m_tileSizeY);

				int tu = tileNumber % x;
				int tv;
				if (y % 2 == 0)
					tv = tileNumber / y;
				else
					tv = tileNumber / (y - 1);

				auto quad = &vertices[(i + j * m_tileMapWidth) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(i * m_tileSizeX, j * m_tileSizeY);
				quad[1].position = sf::Vector2f((i + 1) * m_tileSizeX, j * m_tileSizeY);
				quad[2].position = sf::Vector2f((i + 1) * m_tileSizeX, (j + 1) * m_tileSizeY);
				quad[3].position = sf::Vector2f(i * m_tileSizeX, (j + 1) * m_tileSizeY);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * m_tileSizeX, tv * m_tileSizeY);
				quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSizeX, tv * m_tileSizeY);
				quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSizeX, (tv + 1) * m_tileSizeY);
				quad[3].texCoords = sf::Vector2f(tu * m_tileSizeX, (tv + 1) * m_tileSizeY);


			}
		}

		m_Vertices.push_back(std::move(vertices));
		m_TileData.push_back(layer);

	}

	void TileMap::SetName(const std::string& name)
	{
		m_Name = name;
	}

	const std::string& TileMap::GetName() const
	{
		return m_Name;
	}

	void TileMap::CreateNewLayer()
	{
		sf::VertexArray vertices;
		vertices.setPrimitiveType(sf::Quads);
		vertices.resize(m_tileMapWidth * m_tileMapHeight * 4);

		auto texturePtr = m_tManager->GetTexturePointer(m_textureName);

		for (auto i = 0; i < m_tileMapWidth; i++)
		{
			for (auto j = 0; j < m_tileMapHeight; j++)
			{
				auto quad = &vertices[(i + j * m_tileMapWidth) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(i * m_tileSizeX, j * m_tileSizeY);
				quad[1].position = sf::Vector2f((i + 1) * m_tileSizeX, j * m_tileSizeY);
				quad[2].position = sf::Vector2f((i + 1) * m_tileSizeX, (j + 1) * m_tileSizeY);
				quad[3].position = sf::Vector2f(i * m_tileSizeX, (j + 1) * m_tileSizeY);
			}
		}

		std::vector<int> layer(m_tileMapWidth * m_tileMapHeight, -1);
		m_TileData.push_back(std::move(layer));

		m_Vertices.push_back(std::move(vertices));
	}

	void TileMap::DeleteAllLayers()
	{
		m_Vertices.clear();
		m_TileData.clear();
	}

	void TileMap::DeleteLayer(unsigned int layer)
	{
		if (layer + 1 > m_Vertices.size())
			return;

		m_Vertices.erase(m_Vertices.begin() + layer);
		m_TileData.erase(m_TileData.begin() + layer);
	}

	unsigned int TileMap::GetNumLayers() const
	{
		return m_Vertices.size();
	}

	void TileMap::SetTileSize(unsigned int tileSizex, unsigned int tileSizeY)
	{
		m_tileSizeX = tileSizex;
		m_tileSizeY = tileSizeY;
	}

	void TileMap::SetTileMapDimensions(unsigned int width, unsigned int height)
	{
		m_tileMapWidth = width;
		m_tileMapHeight = height;

	}

	void TileMap::AddTileAt(unsigned int layer, unsigned int i, unsigned int j, unsigned int tileNumber)
	{
		if (layer + 1 > m_Vertices.size())
			return;

		auto& vertexArray = m_Vertices[layer];

		if ((i + j * m_tileMapWidth) * 4 > vertexArray.getVertexCount())
			return;

		if (tileNumber < 0)
			return;



		auto texturePtr = m_tManager->GetTexturePointer(m_textureName);

		if (!texturePtr.has_value())
		{
			std::cout << "Unable to craete tilemap since the texture wasn't found in the available resources.\n";
			return;
		}


		auto x = (texturePtr.value()->getSize().x / m_tileSizeX);
		auto y = (texturePtr.value()->getSize().y / m_tileSizeY);

		int tu = tileNumber % x;
		int tv;
		if (y % 2 == 0)
			tv = tileNumber / y;
		else
			tv = tileNumber / (y - 1);

#ifdef _DEBUG

		std::cout << "x: " << x << ", y: " << y << ", tu: " << tu << ", tv: " << tv << '\n';
#endif

		auto quad = &vertexArray[(i + j * m_tileMapWidth) * 4];

		// define its 4 corners 
		quad[0].position = sf::Vector2f(i * m_tileSizeX, j * m_tileSizeY);
		quad[1].position = sf::Vector2f((i + 1) * m_tileSizeX, j * m_tileSizeY);
		quad[2].position = sf::Vector2f((i + 1) * m_tileSizeX, (j + 1) * m_tileSizeY);
		quad[3].position = sf::Vector2f(i * m_tileSizeX, (j + 1) * m_tileSizeY);

		// define its 4 texture coordinates
		quad[0].texCoords = sf::Vector2f(tu * m_tileSizeX, tv * m_tileSizeY);
		quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSizeX, tv * m_tileSizeY);
		quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSizeX, (tv + 1) * m_tileSizeY);
		quad[3].texCoords = sf::Vector2f(tu * m_tileSizeX, (tv + 1) * m_tileSizeY);

		m_TileData[layer][i + j * m_tileMapWidth] = tileNumber;
	}

	void TileMap::RemoveTileAt(unsigned int layer, unsigned int i, unsigned int j)
	{
		if (layer + 1 > m_Vertices.size())
			return;

		auto& vertexArray = m_Vertices[layer];

		auto quad = &vertexArray[(i + j * m_tileMapWidth) * 4];

		quad[0].texCoords = sf::Vector2f(-1.f, -1.f);
		quad[1].texCoords = sf::Vector2f(-1.f, -1.f);
		quad[2].texCoords = sf::Vector2f(-1.f, -1.f);
		quad[3].texCoords = sf::Vector2f(-1.f, -1.f);

		m_TileData[layer][i + j * m_tileMapWidth] = -1;

	}

	void TileMap::Clear()
	{
		m_Vertices.clear();
		m_textureName.clear();
		m_Name.clear();
		m_TileData.clear();
	}

	void TileMap::FillLayer(unsigned int layer, int tileIndex)
	{
		for (auto i = 0; i < m_tileMapWidth; i++)
		{
			for (auto j = 0; j < m_tileMapHeight; j++)
			{
				AddTileAt(layer, i, j, tileIndex);
			}
		}
	}

	int TileMap::GetTileWidth() const
	{
		return m_tileMapWidth;
	}

	int TileMap::GetTileHeight() const
	{
		return m_tileMapHeight;
	}

	int TileMap::GetTileSizeX() const
	{
		return m_tileSizeX;
	}

	int TileMap::GetTileSizeY() const
	{
		return m_tileSizeY;
	}

	const std::vector<std::vector<int>>& TileMap::GetTileData() const
	{
		return m_TileData;
	}


	void TileMap::CreateTileMap()
	{
		using json = nlohmann::json;

		std::ifstream file(m_tileMapFile);

		if (!file)
		{
			std::cout << "Unable to open tile map file: " << m_tileMapFile << '\n';
			return;
		}

		json root;

		file >> root;

		m_tileSizeX = root["TILE_WIDTH"];
		m_tileSizeY = root["TILE_HEIGHT"];

		m_tileMapWidth = root["WIDTH"];
		m_tileMapHeight = root["HEIGHT"];

		const auto& layers = root["LAYERS"];

		m_textureName = root["TEXTURE_NAME"];
		auto texturePtr = m_tManager->GetTexturePointer(m_textureName);

		if (!texturePtr.has_value())
		{
			std::cout << "Unable to craete tilemap since the texture wasn't found in the available resources.\n";
			return;
		}


		for (const auto& layer : layers)
		{
			const auto& layerData = layer["DATA"];

			sf::VertexArray vertices;
			vertices.setPrimitiveType(sf::Quads);
			vertices.resize(m_tileMapWidth * m_tileMapHeight * 4);

			std::vector<int> layerInfo(m_tileMapWidth * m_tileMapHeight, -1);

			for (auto i = 0; i < m_tileMapWidth; i++)
			{
				for (auto j = 0; j < m_tileMapHeight; j++)
				{
					//int tileNumber = layerData[i + j * m_tileMapWidth] - 1;
					int tileNumber = layerData[i + j * m_tileMapWidth];

					if (tileNumber < 0)
						continue;

					auto x = (texturePtr.value()->getSize().x / m_tileSizeX);
					auto y = (texturePtr.value()->getSize().y / m_tileSizeY);

					int tu = tileNumber % x;
					int tv;
					if (y % 2 == 0)
						tv = tileNumber / y;
					else
						tv = tileNumber / (y - 1);

					auto quad = &vertices[(i + j * m_tileMapWidth) * 4];

					// define its 4 corners
					quad[0].position = sf::Vector2f(i * m_tileSizeX, j * m_tileSizeY);
					quad[1].position = sf::Vector2f((i + 1) * m_tileSizeX, j * m_tileSizeY);
					quad[2].position = sf::Vector2f((i + 1) * m_tileSizeX, (j + 1) * m_tileSizeY);
					quad[3].position = sf::Vector2f(i * m_tileSizeX, (j + 1) * m_tileSizeY);

					// define its 4 texture coordinates
					quad[0].texCoords = sf::Vector2f(tu * m_tileSizeX, tv * m_tileSizeY);
					quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSizeX, tv * m_tileSizeY);
					quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSizeX, (tv + 1) * m_tileSizeY);
					quad[3].texCoords = sf::Vector2f(tu * m_tileSizeX, (tv + 1) * m_tileSizeY);

					layerInfo[(i + j * m_tileMapWidth)] = tileNumber;
				}
			}
			m_TileData.push_back(std::move(layerInfo));

			m_Vertices.push_back(std::move(vertices));
		}

		std::cout << "TileMap " << m_textureName << " Successfully created\n";
	}




}