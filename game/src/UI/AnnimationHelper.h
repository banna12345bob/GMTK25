#pragma once

#include <engine.h>

class AnimationHelper {
public:
	static void Update(Engine::Timestep ts) 
	{ 
		m_TimeMS += ts;
	}

	void StartLerpFloat(float startPoint, float endPoint, float durationSeconds) 
	{
		m_StartTime = m_TimeMS;
		m_Start = startPoint;
		m_End = endPoint;
		m_Duration = durationSeconds;
	}

	float GetLerpFloat()
	{
		float t = (m_TimeMS - m_StartTime) / (m_Duration * 1000.f);
		float lerp = P(t, m_Start, m_End);
		if (std::isnan(t))
			return m_Start;
		if (t < 0.f)
			return m_Start;
		if (t > 1.f)
			return m_End;
		return lerp;
	}

private:
	static float P(float t, float start, float end) 
	{ 
		return ((1 - t) * start) + (t * end);
	}
private:
	float m_Duration, m_Start, m_End;

	float m_StartTime;
	inline static float m_TimeMS;
};