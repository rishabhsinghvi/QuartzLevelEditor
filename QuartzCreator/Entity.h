#ifndef ENTITY_H
#define ENTITY_H

#include<string>

namespace QuartzCreator
{
	struct Entity
	{
		std::string m_EntityType;
		bool m_Moveable;
		std::string m_textureName;
		unsigned int m_SpriteRectX, m_SpriteRectY;
		unsigned int m_SpriteWidth, m_SpriteHeight;
		float m_IniPosX, m_IniPosY;
		float m_IniVelX, m_IniVelY;
		std::string m_AnimationList;
		std::string m_IdleAnimation;
	};
}

#endif