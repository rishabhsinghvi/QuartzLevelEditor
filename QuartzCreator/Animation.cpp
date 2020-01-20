#include "Animation.h"

namespace QuartzCreator
{
	void Animation::SetSprite(sf::Sprite* sprite)
	{
		m_Sprite = sprite;
	}

	void Animation::SetName(const std::string& name)
	{
		m_AnimationName = name;
	}

	const std::string& Animation::GetName() const
	{
		return m_AnimationName;
	}

	void Animation::AddFrame(const Frame& frame)
	{
		m_totalAnimationTime += frame.m_Dur;
		m_Frames.push_back(frame);
	}

	void Animation::AddFrame(Frame&& frame)
	{
		m_totalAnimationTime += frame.m_Dur;
		m_Frames.push_back(std::move(frame));
	}

	void Animation::AddFrame(int x, int y, int w, int h, float dur)
	{
		sf::IntRect rect(x, y, w, h);
		AddFrame({ std::move(rect), dur });
	}

	void Animation::Update(float dt)
	{
		if (m_Frames.size() == 0)
			return;

		if (m_Done)
			return;

		m_currentTime += dt;

		float p = 0.0f;

		for (auto i = 0; i <= m_currentFrameIndex; i++)
		{
			p += m_Frames[i].m_Dur;
		}




		if (m_currentTime >= p)
		{
			if (m_currentFrameIndex == m_Frames.size() - 1 && m_Looped)
			{
				Reset();
			}
			else if (m_currentFrameIndex == m_Frames.size() - 1 && !m_Looped)
			{
				m_Done = true;
				return;
			}
			else
			{
				m_currentFrameIndex += 1;
			}
		}

		m_Sprite->setTextureRect(m_Frames[m_currentFrameIndex].m_Rect);
	}

	float Animation::GetLength() const
	{
		return m_totalAnimationTime;
	}

	void Animation::Reset()
	{
		m_currentFrameIndex = 0;
		m_currentTime = 0.0f;
		m_Done = false;
	}

	void Animation::SetLooping(bool val)
	{
		m_Looped = val;
	}

	bool Animation::IsAnimationDone() const
	{
		return m_Done;
	}

	void Animation::SetAnimationDone(bool val)
	{
		m_Done = val;
	}

	unsigned int Animation::GetNumFrames() const
	{
		return m_Frames.size();
	}

	bool Animation::IsEmpty() const
	{
		return GetNumFrames() == 0;
	}

	std::vector<Frame>& Animation::GetFrameList()
	{
		return m_Frames;
	}

	void Animation::Clear()
	{
		m_Looped = false;
		m_Done = false;
		m_Frames.clear();
		m_totalAnimationTime = 0.0f;
		m_currentTime = 0.0;
		m_currentFrameIndex = 0;
		m_Sprite = nullptr;
		m_AnimationName = std::string();
	}




}