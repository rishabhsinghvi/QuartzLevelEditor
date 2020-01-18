#include "Timer.h"

namespace QuartzCreator
{
	void Timer::reset()
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	void Timer::stop()
	{
		auto endTime = std::chrono::high_resolution_clock::now();
	}

	float Timer::getDeltaT()
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		auto dt = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_startTime).count();

		m_startTime = endTime;

		return dt * 1e-6; // return in seconds
	}


	
}