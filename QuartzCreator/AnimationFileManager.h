#ifndef ANIMATION_FILE_MANAGER_H
#define ANIMATION_FILE_MANAGER_H

#include <unordered_map>
#include <string>

#include "Animation.h"
#include "AnimationManager.h"

namespace QuartzCreator
{
	class AnimationFileManager
	{
	private:
		bool m_DisplayGUI = false;
		bool m_FileCreated = false;
		std::unordered_map<std::string, AnimationFile> m_AnimationFiles;
		AnimationManager* m_aManager;

	public:

		void Init(AnimationManager* m_tManager); 

		void SetDisplay(bool val);

		void NewAnimationFile(const std::string& name);

		void DisplayGUI();


	};
}

#endif