#pragma once

#include <engine.h>

class InterpolationHelper {
public:
	InterpolationHelper(float start)
		: m_Start(start)
	{
	}

	static void Update(Engine::Timestep ts) 
	{ 
		m_TimeMS += ts;
	}

	void StartInterpolation(float startPoint, float endPoint, float durationSeconds) 
	{
		m_StartTime = m_TimeMS;
		m_Start = startPoint;
		m_End = endPoint;
		m_Duration = durationSeconds;
	}

	float GetLinear()
	{
		float t = (m_TimeMS - m_StartTime) / (m_Duration * 1000.f);
		if (std::isnan(t))
			return m_Start;
		if (t < 0.f)
			return m_Start;
		if (t > 1.f)
			return m_End;
		return LinearLerp(t, m_Start, m_End);
	}

	float QuadraticEaseIn()
	{
		float t = (m_TimeMS - m_StartTime) / (m_Duration * 1000.f);
		if (std::isnan(t))
			return m_Start;
		if (t < 0.f)
			return m_Start;
		if (t > 1.f)
			return m_End;
		return QuadraticLerpIn(t, m_Start, m_End);
	}

	float QuadraticEaseOut()
	{
		float t = (m_TimeMS - m_StartTime) / (m_Duration * 1000.f);
		if (std::isnan(t))
			return m_Start;
		if (t < 0.f)
			return m_Start;
		if (t > 1.f)
			return m_End;
		return QuadraticLerpOut(t, m_Start, m_End);
	}

	float CublicEaseIn()
	{
		float t = (m_TimeMS - m_StartTime) / (m_Duration * 1000.f);
		if (std::isnan(t))
			return m_Start;
		if (t < 0.f)
			return m_Start;
		if (t > 1.f)
			return m_End;
		return CubicLerpIn(t, m_Start, m_End);
	}

private:
	static float LinearLerp(float t, float start, float end) 
	{ 
		return ((1 - t) * start) + (t * end);
	}

	static float QuadraticLerpIn(float t, float start, float end)
	{
		return ((1 - t) * start) + (glm::pow(t,2) * end);
	}

	static float QuadraticLerpOut(float t, float start, float end)
	{
		return ((1 - t) * start) + (glm::sqrt(t) * end);
	}

	static float CubicLerpIn(float t, float start, float end)
	{
		return ((1 - t) * start) + (glm::pow(t,3) * end);
	}
private:
	float m_Duration, m_Start, m_End;

	float m_StartTime;
	inline static float m_TimeMS;
};