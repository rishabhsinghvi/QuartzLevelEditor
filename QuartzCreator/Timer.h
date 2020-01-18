#ifndef QUARTZ_TIMER_H
#define QUARTZ_TIMER_H

#include<chrono>

namespace QuartzCreator
{
	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;

	public:

		Timer() = default;

		void reset();

		void stop();

		float getDeltaT();

	};
}

#endif