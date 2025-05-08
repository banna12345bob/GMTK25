#include "GraphicsAPI.h"
#include "platform/OpenGL/OpenGLGraphicsAPI.h"

namespace Engine {

	GraphicsAPI::API GraphicsAPI::s_API = GraphicsAPI::API::OpenGL;

	Scope<GraphicsAPI> GraphicsAPI::Create(Scope<Window>* window)
	{
		switch (s_API)
		{
		case Engine::GraphicsAPI::API::None: EG_CORE_ASSERT(false, "Cannot have GraphicsAPI::None"); return nullptr;
		case Engine::GraphicsAPI::API::OpenGL: EG_CORE_INFO("Current API set to OpenGL"); return CreateScope<OpenGLGraphicsAPI>(window);
		}
		EG_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
