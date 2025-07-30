#pragma once

#include "engine/core/core.h"

namespace Engine {

	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		enum RenderMode {
			Normal,
			Wireframe
		};

	public:
		virtual void Render() = 0;

		void setRenderMode(RenderMode renderMode) { m_RenderMode = renderMode; }
		bool getRenderMode() { return m_RenderMode; }

		static Scope<RenderAPI> Create();
	private:
		RenderMode m_RenderMode = RenderMode::Normal;

		static API s_API;
	};
}

