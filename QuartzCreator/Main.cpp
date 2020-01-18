//#include "imgui.h"
//#include "imgui-SFML.h"
//
//#include <SFML/Graphics/RenderWindow.hpp>
//#include <SFML/System/Clock.hpp>
//#include <SFML/Window/Event.hpp>
//#include <SFML/Graphics/CircleShape.hpp>
//
//#include<iostream>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Quartz Level Creator");
//    window.setFramerateLimit(60);
//    ImGui::SFML::Init(window);
//
//    
//
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    sf::Clock deltaClock;
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            ImGui::SFML::ProcessEvent(event);
//
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//        }
//
//        ImGui::SFML::Update(window, deltaClock.restart());
//
//        // Render stuff here
//
//        
//        bool tool = true;
//        ImGui::Begin("Quartz Level Creator", &tool, ImGuiWindowFlags_MenuBar);
//        ImGui::Text("Enter a value\n");
//        ImGui::End();
//
//
//
//        window.clear();
//        window.draw(shape);
//        ImGui::SFML::Render(window);
//        window.display();
//    }
//
//    ImGui::SFML::Shutdown();
//}



#include "QuartzCreator.h"

#include "Config.h"

#include<iostream>

int main()
{
	using namespace QuartzCreator;

	auto quartzCreatorApp = std::make_unique<QuartzCreatorApp>();

	quartzCreatorApp->init();
	quartzCreatorApp->run();
	quartzCreatorApp->shutdown();
}
