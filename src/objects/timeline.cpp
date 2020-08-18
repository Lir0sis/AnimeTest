#include "timeline.h"

namespace CourseLab
{
	GLfloat TimeLine::s_fps;

	TimeLine* TimeLine::Instance()
	{
		static TimeLine instance;
		return &instance;
	}

	void TimeLine::Update()
	{
		m_lastDelta = GetDelta();
		if (m_isPlaying) 
		{
			m_timeStamp += m_fps * m_lastDelta * m_speedFactor;
			if (m_timeStamp  > m_duration * m_fps) 
			{ 
				if(m_isRepeating)
				m_timeStamp = 0.0f; 
				else
				{
					m_isPlaying = false;
					m_timeStamp = m_duration * m_fps;
				}
			}
		}
		m_frameStamp = std::floor(m_timeStamp);
	}

	void TimeLine::synchroniseStamps() {
		m_timeStamp = m_frameStamp;
	}

	GLfloat TimeLine::GetDelta()
	{
		using Clock = std::chrono::steady_clock;
		using Duration = std::chrono::duration <GLfloat, std::ratio<1>>;
		using TimePoint = std::chrono::steady_clock::time_point;

		static TimePoint lastFrame{ Clock::now() };

		TimePoint end = Clock::now();
		Duration elapsed =  end - lastFrame;

		lastFrame = end;

		return elapsed.count();
	}

	TimeLine::TimeLine() : m_fps(s_fps == 0 ? 24.0f : s_fps) {
		m_lastDelta = 0.0f;

		m_isPlaying = false; m_isRepeating = false;
		
		m_duration = 25.0f;
		m_timeStamp = 0.0f; m_frameStamp = 0;
		LOG("TimeLine created")
	}
	TimeLine::~TimeLine()
	{

	}
}