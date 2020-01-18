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
		bool m_tileMapSelector = false;
		bool m_entityCreator = false;
		bool m_textureSelector = false;
		bool m_textureNamer = false;
		std::unique_ptr<TextureManager> m_textureManager; 
		std::unique_ptr<TileMapManager> m_tileMapManager;
		TileMap* m_currentTileMap = nullptr;

	public:

		QuartzCreatorApp() = default;

		QuartzCreatorApp(const std::string& windowName);

		void init();

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