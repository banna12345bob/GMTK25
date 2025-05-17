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
		/**
		* If turning VSync on, adaptive VSync will be tried first, then regualr VSync if that doesn't work.
		* @returns 0 for off, 1 for VSync and -1 for adaptive VSync
		*/
		virtual int SetVSync(bool value) { return 0; };

		static Scope<GraphicsAPI> Create(Scope<Window>* window);

		static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}

