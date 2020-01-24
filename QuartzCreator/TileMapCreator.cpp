#include "TileMapCreator.h"
#include "TileMap.h"


#include "imgui.h"
#include "imgui-SFML.h"


#include <iostream>

namespace QuartzCreator
{
	extern float OFFSET_X;
	extern float OFFSET_Y;

	void TileMapCreator::Init(TextureManager* tm, TileMapManager* tpm)
	{
		m_TexManager = tm;
		m_TileManager = tpm;
	}

	void TileMapCreator::EnableCreator()
	{
		m_Running = true;
	}

	void TileMapCreator::DisplayGUI(sf::RenderWindow* window)
	{
		if (!m_Running)
			return;

		if (!m_DataAcquired)
			AcquireData();

		else
			CreateTileMap(window);

	}

	void TileMapCreator::AcquireData()
	{
		static char infoName[50];
		ImGui::Begin("Tile Map Info");
		ImGui::InputText("Tile Map Name", infoName, 50);
		ImGui::InputInt("Num Tiles X", &numTilesX);
		ImGui::InputInt("Num Tiles Y", &numTilesY);
		ImGui::InputInt("Tile Size X", &tileSizeX);
		ImGui::InputInt("Tile Size Y", &tileSizeY);
		ImGui::Text("Selected Texture Atlas: %s", m_TextureName.c_str());

		for (const auto& x : m_TexManager->GetTextureList())
		{
			if (ImGui::Selectable(x.first.c_str()))
			{
				m_TextureName = x.first;
			}
		}

		ImGui::Separator();

		if (ImGui::Button("Start"))
		{
			if (numTilesX >= 1 && numTilesY >= 1 && tileSizeX > 0 && tileSizeY > 0 && !m_TextureName.empty() && infoName[0] != '\0')
			{
				m_Name = std::string(infoName);
				m_DataAcquired = true;
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Close"))
		{
			m_DataAcquired = false;
			m_Running = false;
		}
		ImGui::End();
	}

	void TileMapCreator::CreateTileMap(sf::RenderWindow* window)
	{

		static bool gridLinesCreated = false;
		static bool spriteArrayCreated = false;

		static int x;
		static int y;

		static sf::VertexArray gridLines;
		static std::vector<sf::Sprite> sprites;


		
		static TileMap tileMap(m_TexManager);
		static bool tmCreated = false;
		if (!tmCreated)
		{
			tileMap.SetTileSize(tileSizeX, tileSizeY);
			tileMap.setPosition(0.f, 0.f);
			tileMap.SetTileMapDimensions(numTilesX, numTilesY);
			tileMap.SetTextureName(m_TextureName);
			//std::vector<int> layer(numTilesX * numTilesY, 0);
			//tileMap.AddLayer(layer);
			tileMap.CreateNewLayer();
			tmCreated = true;
		}
		if (!gridLinesCreated)
		{
			gridLines.clear();
			gridLines.setPrimitiveType(sf::Lines);

			for (auto i = 0; i <= numTilesX; i++)
			{
				gridLines.append(sf::Vertex(sf::Vector2f(i * tileSizeX, 0.0f)));
				gridLines.append(sf::Vertex(sf::Vector2f((i)*tileSizeX, tileSizeY * numTilesY)));
			}

			for (auto i = 0; i <= numTilesY; i++)
			{
				gridLines.append(sf::Vertex(sf::Vector2f(0.0f, (i * tileSizeY))));
				gridLines.append(sf::Vertex(sf::Vector2f(tileSizeX * numTilesX, tileSizeY * (i))));
			}
			gridLinesCreated = true;
		}

		if (!spriteArrayCreated)
		{
			sprites.clear();

			const auto& tex = m_TexManager->GetLoadedTextureRef(m_TextureName);
			auto sizeX = tex.getSize().x;
			auto sizeY = tex.getSize().y;

			x = sizeX / tileSizeX;
			y = sizeY / tileSizeY;

			for (auto i = 0; i < y; i++)
			{
				for (auto j = 0; j < x; j++)
				{
					sf::Sprite spr;
					spr.setTexture(tex);
					spr.setTextureRect(sf::IntRect(j * tileSizeX, i * tileSizeY, tileSizeX, tileSizeY));
					sprites.push_back(std::move(spr));
				}
			}

			spriteArrayCreated = true;

		}

		ImGui::Begin("Select Tile");

		for (auto i = 0; i < y; i++)
		{
			for (auto j = 0; j < x; j++)
			{
				auto& s = sprites[i * x + j];
				ImGui::PushID(i * x + j);
				if (ImGui::ImageButton(s))
				{
					m_Cur.m_Sprite = s;
					//m_Cur.m_Sprite.setColor(sf::Color(255, 255, 255, 128));
					m_Cur.m_Sprite.setColor(sf::Color(0, 0, 0, 128));
					m_Cur.m_tileIndex = i * x + j;
				}
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
		}


		ImGui::Separator();
		ImGui::Text("Current Layer: %d", m_Cur.currentLayer);
		ImGui::Separator();
		for (auto i = 0; i < tileMap.GetNumLayers(); i++)
		{
			auto str = "Layer " + std::to_string(i);
			if (ImGui::Selectable(str.c_str()))
			{
				m_Cur.currentLayer = i;
			}
		}

		ImGui::Separator();

		if (ImGui::Button("Fill"))
		{
			if (m_Cur.m_tileIndex >= 0)
			{
				tileMap.FillLayer(m_Cur.currentLayer, m_Cur.m_tileIndex);
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Layer"))
		{
			tileMap.CreateNewLayer();
		}

		ImGui::SameLine();
		
		if (ImGui::Button("Clear Map"))
		{
			tileMap.DeleteAllLayers();
			tileMap.CreateNewLayer();
			m_Cur.currentLayer = 0;
		}

		ImGui::Separator();
		
		if (ImGui::Button("Save TileMap"))
		{

		}


		ImGui::Separator();

		if (ImGui::Button("Close"))
		{
			m_Running = false;
			m_DataAcquired = false;
			tmCreated = false;
			gridLinesCreated = false;
			spriteArrayCreated = false;

			tileMap.Clear();

			m_Cur.currentLayer = 0;
			m_Cur.m_tileIndex = 0;
		}
		ImGui::End();

		auto mousePos = sf::Mouse::getPosition(*window);

		auto mousePosWorldCo = window->mapPixelToCoords(mousePos);

		int valX = mousePosWorldCo.x / tileSizeX;
		int valY = mousePosWorldCo.y / tileSizeY;

		if (valX >= 0 && valX < numTilesX && valY >= 0 && valY < numTilesY)
		{
			m_Cur.m_Sprite.setPosition(static_cast<float>(valX * tileSizeX), static_cast<float>(valY * tileSizeY));
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
				tileMap.AddTileAt(m_Cur.currentLayer, valX, valY, m_Cur.m_tileIndex);

		}

		


		window->draw(tileMap);

		window->draw(gridLines);
	}




}