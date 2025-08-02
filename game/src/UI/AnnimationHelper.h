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
		if (!m_StartTime)
			m_StartTime = m_TimeMS;
		m_Start = startPoint;
		m_End = endPoint;
		m_Duration = durationSeconds;
	}

	float GetLerpFloat()
	{
		float lerp = P((m_TimeMS - m_StartTime) / (m_Duration * 1000.f), m_Start, m_End);
		return lerp < m_End ? lerp : m_End;
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