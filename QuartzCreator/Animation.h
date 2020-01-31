#ifndef ANIMATION_H
#define ANIMATION_H

#include "SFML/Graphics.hpp"

#include <vector>
#include <string>

namespace QuartzCreator
{
	struct Frame
	{
		sf::IntRect m_Rect;
		float m_Dur;

	};
	class Animation
	{
	private:
		bool m_Looped = false;
		bool m_Done = false;
		std::vector<Frame> m_Frames;
		float m_totalAnimationTime;
		float m_currentTime = 0.0f;
		float m_currentFrameIndex = 0;
		sf::Sprite* m_Sprite;
		std::string m_AnimationName;
		
	public: 

		Animation() = default;

		void SetSprite(sf::Sprite* sprite);

		void SetName(const std::string& name);

		const std::string& GetName() const;

		void AddFrame(const Frame& frame);
		void AddFrame(Frame&& frame);
		void AddFrame(int x, int y, int w, int h, float dur);

		void Update(float dt);

		float GetLength() const;

		void Reset();

		void SetLooping(bool val);

		bool IsAnimationDone() const;

		void SetAnimationDone(bool val);

		unsigned int GetCurrentFrameIndex() const;

		unsigned int GetNumFrames() const;

		bool IsEmpty() const;

		std::vector<Frame>& GetFrameList();

		void RemoveFrame(unsigned int index);

		void Clear();
	};

	struct AnimationFile
	{
		std::string m_FileName;
		std::vector<Animation> m_Animations;
	};
}

#endif