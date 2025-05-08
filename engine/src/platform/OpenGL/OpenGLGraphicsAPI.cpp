#include "OpenGLGraphicsAPI.h"


namespace Engine {

	OpenGLGraphicsAPI::OpenGLGraphicsAPI(Scope<Window>* window)
	{
		m_Window = window;
		m_SpriteRenderer = new GLSpriteRenderer();
	}

	void OpenGLGraphicsAPI::Render()
	{
		m_SpriteRenderer->Render();
		m_Window->get()->GL_SwapWindow();
	}
}
