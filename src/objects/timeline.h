#pragma once

#include <../main.h>

namespace CourseLab
{
	class TimeLine {
	private:
		const GLfloat m_fps;
		GLfloat m_duration;
		GLfloat m_timeStamp;
		GLint m_frameStamp;

		GLfloat m_lastDelta{0.0f};
		GLfloat m_speedFactor{1.0f};
		bool m_isPlaying;
		bool m_isRepeating;
		//UI

	public:
		TimeLine(const TimeLine&) = delete;
		const TimeLine& operator=(const TimeLine&) = delete;

		static TimeLine* Instance();

		inline GLfloat GetCurrentFrame() { return m_timeStamp; }

		inline static void SetFPS(GLfloat f = 24.0f) {
			s_fps = f;
		}

		void Update();

		void Play() { m_isPlaying = true; }
		inline void Pause() { m_isPlaying = false; }
		inline void Stop() { Pause(); m_timeStamp = 0; }
		inline void SetSpeed(GLfloat factor) { m_speedFactor = factor; }
		inline GLfloat GetFPS() { return m_fps; }

	private:
		void synchroniseStamps();
		GLfloat GetDelta();
		static GLfloat s_fps;
		TimeLine();
		~TimeLine();

		friend class Gui;
	};
}