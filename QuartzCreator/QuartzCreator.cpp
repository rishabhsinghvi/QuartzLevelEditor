#include "QuartzCreator.h"

#ifdef _DEBUG
#include<iostream>
#endif



namespace QuartzCreator
{
	static constexpr size_t MAX_T_LEN = 50;
	extern const unsigned int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
	extern const unsigned int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;

	constexpr float KEY_MOVE_SPEED = 10.f;

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
		m_animationManager = std::make_unique<AnimationManager>();
		m_Browser = std::make_unique<imgui_addons::ImGuiFileBrowser>();
		m_Timer = std::make_unique<sf::Clock>();
		m_Config = std::make_unique<Config>();

		m_View = m_Window->getDefaultView();

		m_tileMapManager->init(m_textureManager.get());
		m_animationManager->Init(m_textureManager.get());

		m_Config->loadConfigFromFile();

		m_textureManager->loadConfigData(m_Config->getTextureRecords());
		m_tileMapManager->loadConfigData(m_Config->getTileMapRecords());
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

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				m_View.move(KEY_MOVE_SPEED, 0.f);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				m_View.move(-KEY_MOVE_SPEED, 0.f);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				m_View.move(0.f, -KEY_MOVE_SPEED);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				m_View.move(0.f, KEY_MOVE_SPEED);
			}

		

			ImGui::SFML::Update(*m_Window, m_Timer->restart());

			m_Window->clear();
			m_Window->setView(m_View);
			

			drawTileMap();
			drawGUI();

			ImGui::SFML::Render(*m_Window);

			m_Window->display();

		}
	}

	void QuartzCreatorApp::shutdown()
	{
		m_Config->writeConfigToFile();
		ImGui::SFML::Shutdown();
	}

	void QuartzCreatorApp::drawGUI()
	{
		ImGui::Begin("Quartz Level Creator", &m_Running, ImGuiWindowFlags_MenuBar);
		// Menu
		drawMenu();

		// Show available resources
		drawResourcesList();


		m_animationManager->DisplayGUI(m_Timer->getElapsedTime().asSeconds());
		
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
				if (ImGui::MenuItem("New Entity..."))
				{

				}
				if (ImGui::MenuItem("New Animation ..."))
				{
					m_animationManager->SetDisplay(true);
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
			const auto& textures = m_textureManager->getTextureList();

			
			static char name[MAX_T_LEN] = "default";

			ImGui::Text("Enter Name for TileMap: ");
			ImGui::InputText("Tilemap Name", name, MAX_T_LEN);

			ImGui::Separator();
			ImGui::Text("Select Texture for TileMap:\n");
			for (const auto& texture : textures)
			{
				if (ImGui::Selectable(texture.first.c_str()))
				{
					auto sName = std::string(name);
					m_Config->addToConfig(Config::TileMapRecord(sName, texture.first, m_Browser->selected_fn));
					m_tileMapManager->loadTileMap(sName, m_Browser->selected_fn, texture.first);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
			

		}



		if (ImGui::BeginPopup("TextureNamer"))
		{
			
			static char name[MAX_T_LEN] = "default";

			ImGui::Text("Enter Name for Texture: ");
			ImGui::InputText("Texture Name", name, MAX_T_LEN);

			if (ImGui::Button("Create Texture"))
			{
				if (name[0] != '\0')
				{
					auto sName = std::string(name);
					m_Config->addToConfig(Config::TextureRecord(sName, m_Browser->selected_fn));
					m_textureManager->loadTexture(sName, m_Browser->selected_fn);
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
				/*if (ImGui::Selectable(map.first.c_str()))
				{
					m_currentTileMap = m_tileMapManager->getTileMapPointer(map.first);
				}*/
				if (ImGui::TreeNode(map.first.c_str()))
				{
					if (ImGui::Button("Load Tile Map"))
					{
						m_currentTileMap = m_tileMapManager->getTileMapPointer(map.first);
					}
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();

			ImGui::Separator();
		}



		if (ImGui::TreeNode("Available Textures"))
		{
			static bool renameTexture = false;
			static bool imageDisplay = false;
			static std::string imageName;

			const auto& textures = m_textureManager->getTextureList();

			for (const auto& texture : textures)
			{
				/*if (ImGui::Selectable(texture.first.c_str()))
				{
					imageDisplay = true;
					imageName = texture.first;
				}*/
				if (ImGui::TreeNode(texture.first.c_str()))
				{
					if (ImGui::Button("Show Texture"))
					{
						imageDisplay = true;
						imageName = texture.first;
					}

					if (ImGui::Button("Rename Texture"))
					{
						renameTexture = true;
						imageName = texture.first;
					}
					ImGui::TreePop();
				}

			}

			if (renameTexture)
			{
				ImGui::OpenPopup("RenameTexture");
				renameTexture = false;
			}

			if (ImGui::BeginPopup("RenameTexture"))
			{
				ImGui::Text("Enter New Name For Texture");
				
				static char name[MAX_T_LEN];

				ImGui::InputText("New Name", name, MAX_T_LEN);

				if (ImGui::Button("Rename"))
				{
					if (name[0] != '\0')
					{
						auto sName = std::string(name);

						auto& record = m_Config->getTextureRecord(imageName);
						record.m_Name = sName;

						m_Config->changeTextureName(imageName, sName);

						m_textureManager->renameTexture(imageName, sName);
						m_tileMapManager->changeTextureName(imageName, sName);

						imageName = sName;
					
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();

			}



			if (imageDisplay)
			{
				ImGui::Begin(imageName.c_str(), &imageDisplay);
				auto& t = m_textureManager->getLoadedTextureRef(imageName);
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

		if (ImGui::TreeNode("Available Animations"))
		{
			const auto& anims = m_animationManager->GetAnimationList();

			for (const auto& anim : anims)
			{
				if (ImGui::TreeNode(anim.first.c_str()))
				{

					ImGui::TreePop();
				}
			}
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