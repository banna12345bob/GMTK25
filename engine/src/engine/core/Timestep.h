#pragma once

#include <chrono>

namespace Engine {

	class Timestep
	{
	public:
		Timestep(float time = 0.f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
	private:
		float m_Time;
	};
}