#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include "Animation.h"
#include "TextureManager.h"

#include <unordered_map>
#include<string>


namespace QuartzCreator
{
	class AnimationManager
	{
	private:
		std::unordered_map<std::string, Animation> m_AnimationList;
		bool m_DisplayGUI = false;
		TextureManager* m_tManager;

		// Keeps tracker of current animation creation
		// Could potentially change them to static local variables; revisit later
		std::string m_CurrentTextureName;
		bool m_DisplayMaker = false;
		sf::Sprite m_ViewSprite;
		std::string m_AnimName;

	public:

		void Init(TextureManager* tManager);

		const std::unordered_map<std::string, Animation>& GetAnimationList() const;

		void SetDisplay(bool val);

		void DisplayGUI(float dt);

	private:

		void NewAnimation();
		
		void MakeAnimation(float dt);


	};
}

#endif