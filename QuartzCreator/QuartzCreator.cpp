#include "QuartzCreator.h"

#ifdef _DEBUG
#include<iostream>
#endif

namespace QuartzCreator
{
	extern const unsigned int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
	extern const unsigned int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;

	QuartzCreatorApp::QuartzCreatorApp(const std::string& windowName) :
		m_WindowName(windowName)
	{
	}

	void QuartzCreatorApp::init()
	{
		m_Window = std::make_unique<sf::RenderWindow>(sf::VideoMode().getDesktopMode(), m_WindowName, sf::Style::Fullscreen);
		m_Window->setFramerateLimit(60);
		ImGui::SFML::Init(*m_Window);

		m_textureManager = std::make_unique<TextureManager>();
		m_tileMapManager = std::make_unique<TileMapManager>();
		m_Browser = std::make_unique<imgui_addons::ImGuiFileBrowser>();
		m_Timer = std::make_unique<sf::Clock>();


		m_tileMapManager->init(m_textureManager.get());
	}

	void QuartzCreatorApp::run()
	{
		while (m_Running)
		{
			sf::Event event;
			while (m_Window->pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);

				if (event.type == sf::Event::Closed)
				{
					m_Running = false;
				}
			}

			ImGui::SFML::Update(*m_Window, m_Timer->restart());

			drawTileMap();
			drawGUI();

			m_Window->clear();

			ImGui::SFML::Render(*m_Window);

			m_Window->display();

		}
	}

	void QuartzCreatorApp::shutdown()
	{
		ImGui::SFML::Shutdown();
	}

	void QuartzCreatorApp::drawGUI()
	{
		ImGui::Begin("Quartz Level Creator", &m_Running, ImGuiWindowFlags_MenuBar);
		// Menu
		drawMenu();

		// Show available resources
		drawResourcesList();
		
		ImGui::End();

	}
	void QuartzCreatorApp::drawMenu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Tilemap...", "Ctrl+T"))
				{
					m_tileMapSelector = true;
				}
				if (ImGui::MenuItem("Open Texture...", "Ctrl+O"))
				{
					m_textureSelector = true;
				}
				if (ImGui::MenuItem("Create New Entity..."))
				{

				}
				if (ImGui::MenuItem("Close..."))
				{
					m_Running = false;
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (m_tileMapSelector)
		{
			ImGui::OpenPopup("TileMapSelector");
			m_tileMapSelector = false;
		}

		if (m_textureSelector)
		{
			ImGui::OpenPopup("TextureSelector");
			m_textureSelector = false;
		}


		if (m_Browser->showOpenFileDialog("TileMapSelector", ImVec2(700, 310), ".json"))
		{
			ImGui::OpenPopup("TileMapTextureSelector");
		}

		if (m_Browser->showOpenFileDialog("TextureSelector", ImVec2(700, 310), ".png"))
		{
			ImGui::OpenPopup("TextureNamer");

		}


		if (ImGui::BeginPopup("TileMapTextureSelector"))
		{
			ImGui::Text("Select Texture for TileMap");
			const auto& textures = m_textureManager->getTextureList();

			for (const auto& texture : textures)
			{
				if (ImGui::Selectable(texture.first.c_str()))
				{
					m_tileMapManager->loadTileMap(texture.first, m_Browser->selected_fn, texture.first);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
			
		}



		if (ImGui::BeginPopup("TextureNamer"))
		{
			static constexpr size_t MAX_T_LEN = 50;
			static char name[MAX_T_LEN] = "default";

			ImGui::Text("Enter Name for Texture: ");
			ImGui::InputText("Texture Name", name, MAX_T_LEN);

			if (ImGui::Button("Create Texture"))
			{
				if (name[0] != '\0')
				{
					m_textureManager->loadTexture(std::string(name), m_Browser->selected_fn);
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}
	}
	void QuartzCreatorApp::drawResourcesList()
	{
		if (ImGui::TreeNode("Available TileMaps"))
		{
			const auto& maps = m_tileMapManager->getTileMapList();

			for (const auto& map : maps)
			{
				if (ImGui::Selectable(map.first.c_str()))
				{
					m_currentTileMap = m_tileMapManager->getTileMapPointer(map.first);
				}
			}

			ImGui::TreePop();

			ImGui::Separator();
		}



		if (ImGui::TreeNode("Avaiable Textures"))
		{
			static bool imageDisplay = false;
			static std::string imageName;

			const auto& textures = m_textureManager->getTextureList();

			for (const auto& texture : textures)
			{
				if (ImGui::Selectable(texture.first.c_str()))
				{
					imageDisplay = true;
					imageName = texture.first;
				}
			}

			if (imageDisplay)
			{
				ImGui::Begin(imageName.c_str(), &imageDisplay);
				auto& t = m_textureManager->getTextureRef(imageName);
				ImGui::Text("Size (in px) = %d x %d", t.getSize().x, t.getSize().y);
				ImGui::Image(t);
				ImGui::End();
			}



			ImGui::TreePop();
			ImGui::Separator();
		}

		if (ImGui::TreeNode("Available Entities"))
		{
			ImGui::TreePop();

			ImGui::Separator();
		}



	}
	void QuartzCreatorApp::drawTileMap()
	{
		if (m_currentTileMap)
		{
			m_Window->draw(*m_currentTileMap);
		}
	}
}