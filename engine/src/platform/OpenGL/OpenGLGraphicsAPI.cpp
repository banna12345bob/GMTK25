#include "OpenGLGraphicsAPI.h"

#include "engine/debug/Instrumentor.h"

namespace Engine {

	OpenGLGraphicsAPI::OpenGLGraphicsAPI(Scope<Window>* window)
	{
		m_Window = window;
		m_SpriteRenderer = new GLSpriteRenderer();
	}

	void OpenGLGraphicsAPI::Render()
	{
		EG_PROFILE_FUNCTION();
		m_Window->get()->GL_SwapWindow();
		m_SpriteRenderer->Render();
	}
}
