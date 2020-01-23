#ifndef QUARTZ_CREATOR_H
#define QUARTZ_CREATOR_H

#include<string>
#include<memory>

#include "SFML/Graphics.hpp"

#include "ext/FileBrowser/ImGuiFileBrowser.h"
#include "imgui.h"
#include "imgui-SFML.h"

#include "Timer.h"
#include "TextureManager.h"
#include "TileMapManager.h"
#include "AnimationManager.h"
#include "TileMapCreator.h"
#include "Config.h"

namespace QuartzCreator
{
	class QuartzCreatorApp
	{
	private:
		bool m_Running = true;
		std::string m_WindowName= "Quartz Creator";
		std::unique_ptr<sf::RenderWindow> m_Window;
		std::unique_ptr<sf::Clock> m_Timer;
		std::unique_ptr<imgui_addons::ImGuiFileBrowser> m_Browser;
		std::unique_ptr<Config> m_Config;
		bool m_tileMapSelector = false;
		bool m_entityCreator = false;
		bool m_textureSelector = false;
		bool m_textureNamer = false;
		std::unique_ptr<TextureManager> m_textureManager; 
		std::unique_ptr<TileMapManager> m_tileMapManager;
		std::unique_ptr<AnimationManager> m_animationManager;
		std::unique_ptr<TileMapCreator> m_tmCreator;
		TileMap* m_currentTileMap = nullptr;
		sf::View m_View;

	public:

		QuartzCreatorApp() = default;

		QuartzCreatorApp(const std::string& windowName);

		void Init();

		void run();

		void shutdown();
		
	private:

		void drawGUI();

		void drawMenu();

		void drawResourcesList();

		void drawTileMap();

	};
}

#endif