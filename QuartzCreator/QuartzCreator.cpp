#include "QuartzCreator.h"

#ifdef _DEBUG
#include<iostream>
#endif

#include<cmath>



namespace QuartzCreator
{
	extern float OFFSET_X = 0.0f;
	extern float OFFSET_Y = 0.0f;

	static constexpr size_t MAX_T_LEN = 50;
	extern const unsigned int WINDOW_WIDTH = sf::VideoMode::getDesktopMode().width;
	extern const unsigned int WINDOW_HEIGHT = sf::VideoMode::getDesktopMode().height;

	constexpr float KEY_MOVE_SPEED = 10.f;

	QuartzCreatorApp::QuartzCreatorApp(const std::string& windowName) :
		m_WindowName(windowName)
	{
	}

	void QuartzCreatorApp::Init()
	{
		m_Window = std::make_unique<sf::RenderWindow>(sf::VideoMode().getDesktopMode(), m_WindowName, sf::Style::Fullscreen);
		m_Window->setFramerateLimit(60);
		ImGui::SFML::Init(*m_Window);

		m_textureManager = std::make_unique<TextureManager>();
		m_tileMapManager = std::make_unique<TileMapManager>();
		m_animationManager = std::make_unique<AnimationManager>();
		m_tmCreator = std::make_unique<TileMapCreator>();

		m_Browser = std::make_unique<imgui_addons::ImGuiFileBrowser>();
		m_Timer = std::make_unique<sf::Clock>();
		m_Config = std::make_unique<Config>();

		m_View = m_Window->getDefaultView();

		m_tileMapManager->Init(m_textureManager.get());
		m_animationManager->Init(m_textureManager.get());

		m_tmCreator->Init(m_textureManager.get(), m_tileMapManager.get());

		m_Config->LoadConfigFromFile();

		m_textureManager->LoadConfigData(m_Config->GetTextureRecords());
		m_tileMapManager->LoadConfigData(m_Config->GetTileMapRecords());
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

				if (event.type == sf::Event::MouseWheelScrolled)
				{
					float delta = -1.0f * static_cast<float>(event.mouseWheelScroll.delta);
					auto zoomLevel = std::exp(delta);
					m_View.zoom(zoomLevel);
				}

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				m_View.move(KEY_MOVE_SPEED, 0.f);
				OFFSET_X += KEY_MOVE_SPEED;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				m_View.move(-KEY_MOVE_SPEED, 0.f);
				OFFSET_X -= KEY_MOVE_SPEED;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				m_View.move(0.f, -KEY_MOVE_SPEED);
				OFFSET_Y -= KEY_MOVE_SPEED;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				m_View.move(0.f, KEY_MOVE_SPEED);
				OFFSET_Y += KEY_MOVE_SPEED;
			}

			ImGui::SFML::Update(*m_Window, m_Timer->restart());

			m_Window->clear(sf::Color(171, 183, 183, 1));
			m_Window->setView(m_View);
			

			drawTileMap();
			drawGUI();

			ImGui::SFML::Render(*m_Window);

			m_Window->display();

		}
	}

	void QuartzCreatorApp::shutdown()
	{
		m_Config->WriteConfigToFile();
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
		m_tmCreator->DisplayGUI(m_Window.get());
		
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
			const auto& textures = m_textureManager->GetTextureList();

			
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
					m_Config->AddToConfig(Config::TileMapRecord(sName, texture.first, m_Browser->selected_fn));
					m_tileMapManager->LoadTileMap(sName, m_Browser->selected_fn, texture.first);
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
					m_Config->AddToConfig(Config::TextureRecord(sName, m_Browser->selected_fn));
					m_textureManager->LoadTexture(sName, m_Browser->selected_fn);
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
			const auto& maps = m_tileMapManager->GetTileMapList();

			for (const auto& map : maps)
			{
				if (ImGui::TreeNode(map.first.c_str()))
				{
					ImGui::Text("Texture Used: %s", map.second.m_TextureName.c_str());
					if (ImGui::Button("Load Tile Map"))
					{
						m_currentTileMap = m_tileMapManager->GetTileMapPointer(map.first);
					}
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();

			ImGui::Separator();
		}



		if (ImGui::TreeNode("Available Textures"))
		{
			static bool imageDisplay = false;
			static bool deleteTexture = false;

			static std::string imageName;

			const auto& textures = m_textureManager->GetTextureList();

			for (const auto& texture : textures)
			{
				if (ImGui::TreeNode(texture.first.c_str()))
				{
					if (ImGui::Button("Show"))
					{
						imageDisplay = true;
						imageName = texture.first;
					}

					ImGui::SameLine();

					if (ImGui::Button("Delete"))
					{
						deleteTexture = true;
						
						// Stop showing image if we're deleting it
						imageDisplay = false;

						imageName = texture.first;
					}

					ImGui::TreePop();
				}

			}


			if (imageDisplay)
			{
				ImGui::Begin(imageName.c_str(), &imageDisplay);
				auto& t = m_textureManager->GetLoadedTextureRef(imageName);
				ImGui::Text("Size (in px) = %d x %d", t.getSize().x, t.getSize().y);
				ImGui::Image(t);
				ImGui::End();
			}

			if (deleteTexture)
			{
				m_textureManager->DeleteTexture(imageName);
				deleteTexture = false;
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

		if (ImGui::TreeNode("Available Animation Files"))
		{
			ImGui::TreePop();
			ImGui::Separator();
		}



		ImGui::NewLine();
		ImGui::NewLine();

		ImGui::Separator();

		if (ImGui::Button("Create Tile Map"))
		{
			m_currentTileMap = nullptr;
			m_tmCreator->EnableCreator();
		}

		ImGui::Separator();

		if (ImGui::Button("Create Entity"))
		{
			
		}

		ImGui::Separator();

		if (ImGui::Button("Create Level"))
		{

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