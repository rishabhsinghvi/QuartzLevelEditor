#include "AnimationManager.h"
#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"

#include<iostream>

namespace QuartzCreator
{
	static constexpr size_t MAX_T_LEN = 50;

	void AnimationManager::Init(TextureManager* tManager)
	{
		m_tManager = tManager;
	}

	const std::unordered_map<std::string, Animation>& AnimationManager::GetAnimationList() const
	{
		return m_AnimationList;
	}
	void AnimationManager::SetDisplay(bool val)
	{
		m_DisplayGUI = val;
	}
	void AnimationManager::NewAnimation()
	{
		static bool nameSelected = false; 
		static std::string animName;

		if (!nameSelected)
		{
			static char name[MAX_T_LEN];
			ImGui::Begin("Animation Name", &m_DisplayGUI);
			ImGui::InputText("Name", name, MAX_T_LEN);
			if (ImGui::Button("Create Animation"))
			{
				if (name[0] != '\0')
				{
					animName = std::string(name);
					m_AnimName = animName;
					nameSelected = true;
					std::cout << animName << '\n';
				}
			}
			ImGui::End();
		}
		else
		{
			const auto& textures = m_tManager->GetTextureList();
			ImGui::Begin("Select Texture");
			for (const auto& texture : textures)
			{
				if (ImGui::Selectable(texture.first.c_str()))
				{
					m_CurrentTextureName = texture.first;
					m_DisplayMaker = true;
					m_DisplayGUI = false;
					nameSelected = false;
				}

			}

			if (ImGui::Button("Close"))
			{
				nameSelected = false;
				animName.clear();
				m_CurrentTextureName.clear();
				m_DisplayMaker = false;
				m_DisplayGUI = false;
				nameSelected = false;
			}
			ImGui::End();
		}
	}
	void AnimationManager::MakeAnimation(float dt)
	{
		static float spriteWidth = 100.f;
		static float spriteHeight = 100.f;

		static std::vector<sf::Sprite> sprites;
		static std::vector<Frame> frames;

		static bool dimensionsChanged = true;
		static bool viewAnimation = false;

		static sf::Clock clock;

		
		auto& tex = m_tManager->GetLoadedTextureRef(m_CurrentTextureName);

		auto sizeX = tex.getSize().x;
		auto sizeY = tex.getSize().y;

		int x = sizeX / spriteWidth;
		int y = sizeY / spriteHeight;

		m_ViewSprite.setTexture(tex);
		
		static Animation animation;
		animation.SetSprite(&m_ViewSprite);
		animation.SetLooping(true);

		if (dimensionsChanged)
		{
			sprites.clear();
			for (auto i = 0; i < y; i++)
			{
				for (auto j = 0; j < x; j++)
				{
					sf::Sprite spr;
					spr.setTexture(tex);
					spr.setTextureRect(sf::IntRect(j * spriteWidth, i * spriteHeight, spriteWidth, spriteHeight));
					sprites.push_back(std::move(spr));
				}
			}
			dimensionsChanged = false;
		}

		ImGui::Begin("Make Animation");

		if (ImGui::InputFloat("Width", &spriteWidth) || ImGui::InputFloat("Height", &spriteHeight))
		{
			dimensionsChanged = true;
		}

		for (auto i = 0; i < y; i++)
		{
			for (auto j = 0; j < x; j++)
			{
				auto s = sprites[i * x + j];
				ImGui::PushID(i * x + j);
				if (ImGui::ImageButton(s))
				{
					animation.AddFrame({ s.getTextureRect(), 1.0f });
					std::cout << animation.GetLength() << '\n';
				}
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
		}

		if (ImGui::Button("View Animation"))
			viewAnimation = true;
		ImGui::SameLine();
		ImGui::Text("Frames: %d", animation.GetNumFrames());
		ImGui::SameLine();
		ImGui::Text("Animation Time: %.3fs", animation.GetLength());

		ImGui::Separator();
		if (ImGui::Button("Close"))
		{
			m_CurrentTextureName.clear();
			m_AnimName.clear();
			
			m_DisplayGUI = false;
			m_DisplayMaker = false;
		}


		auto& frameList = animation.GetFrameList();


		for (auto i = 0; i < frameList.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::Text(std::to_string(i).c_str());
			ImGui::SameLine();
			ImGui::InputFloat("Frame Length", &frameList[i].m_Dur);
			ImGui::SameLine();
			if (ImGui::Button("Remove Frame"))
			{
				animation.RemoveFrame(i);
			}
			ImGui::PopID();

		}

		ImGui::Separator();
		
		if (ImGui::Button("Save Animation"))
		{
			m_AnimationList[m_AnimName] = animation;
			animation.Clear();
			m_CurrentTextureName.clear();
			m_AnimName.clear();

			m_DisplayGUI = false;
			m_DisplayMaker = false;
		}


		if (!animation.IsEmpty() && viewAnimation)
		{
			animation.Update(clock.getElapsedTime().asSeconds());;
			ImGui::Begin("Animation Viewer", &viewAnimation);
			ImGui::Image(m_ViewSprite);
			ImGui::Text("Frame: %d", animation.GetCurrentFrameIndex());
			if (ImGui::Button("Restart Animation"))
			{
				animation.Reset();
			}

			ImGui::Separator();

			if (ImGui::Button("Clear Animation"))
			{
				animation.Clear();
			}


			clock.restart();
			ImGui::End();
		}

		ImGui::End();

	}
	void AnimationManager::DisplayGUI(float dt)
	{
		if (!m_DisplayGUI && !m_DisplayMaker)
			return;

		if (m_DisplayGUI)
			NewAnimation();

		else if (m_DisplayMaker)
			MakeAnimation(dt);
	}
}