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
	public:
		virtual void Render() = 0;

		static Scope<RenderAPI> Create();
	private:
		static API s_API;
	};
}

