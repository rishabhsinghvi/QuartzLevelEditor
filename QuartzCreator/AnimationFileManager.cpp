#include "AnimationFileManager.h"

#include "imgui.h"
#include "imgui-SFML.h"

namespace QuartzCreator
{
	static constexpr size_t MAX_T_LEN = 50;

	void AnimationFileManager::Init(AnimationManager* tManager)
	{
		m_aManager = tManager;
	}

	void AnimationFileManager::SetDisplay(bool val)
	{
		m_DisplayGUI = val;
	}

	void AnimationFileManager::NewAnimationFile(const std::string& name)
	{
		AnimationFile newFile;


	}
	void AnimationFileManager::DisplayGUI()
	{
		if (!m_DisplayGUI)
			return;
		
		if (!m_FileCreated)
		{
			static char name[MAX_T_LEN];

			ImGui::OpenPopup("FileNamer");

			if (ImGui::BeginPopup("FileNamer"))
			{
				ImGui::InputText("File Name", name, MAX_T_LEN);
				if (ImGui::Button("Create File"))
				{
					if (name[0] != '\0')
					{
						ImGui::CloseCurrentPopup();
						NewAnimationFile(name);
						m_DisplayGUI = false;

					}
				}
				ImGui::EndPopup();
			}
		}

	}
}