#pragma once

#include "engine/core/core.h"

#include <glm/glm.hpp>

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
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		void setRenderMode(RenderMode renderMode) { m_RenderMode = renderMode; }
		bool getRenderMode() { return m_RenderMode; }

		static RenderAPI::API getAPI();

		static Scope<RenderAPI> Create();

	private:
		RenderMode m_RenderMode = RenderMode::Normal;

		static RenderAPI::API s_API;
	};
}

