#pragma once
#include "engine/core/core.h"
#include "engine/core/Window.h"

namespace Engine {

	class GraphicsAPI
	{
	public:
		enum class API {
			None = 0, OpenGL = 1
		};

	public:
		virtual void Render() {}

		static Scope<GraphicsAPI> Create(Scope<Window>* window);

		static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}

