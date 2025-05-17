#include "OpenGLGraphicsAPI.h"

#include "engine/debug/Instrumentor.h"

#include "GLSpriteRenderer.h"

namespace Engine {

	OpenGLGraphicsAPI::OpenGLGraphicsAPI(Scope<Window>* window)
	{
		m_Window = window;
		GLSpriteRenderer();
	}

	void OpenGLGraphicsAPI::Render()
	{
		EG_PROFILE_FUNCTION();
		m_Window->get()->GL_SwapWindow();
	}

	void OpenGLGraphicsAPI::SetVSync(bool value) {
		GLSpriteRenderer::SetVSync(value);
	}

	int OpenGLGraphicsAPI::GetVSync()
	{
		return GLSpriteRenderer::GetVSync();
	}
}
