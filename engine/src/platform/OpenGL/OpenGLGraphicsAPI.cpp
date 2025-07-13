#include "OpenGLGraphicsAPI.h"

#include "engine/debug/Instrumentor.h"

namespace Engine {

	OpenGLGraphicsAPI::OpenGLGraphicsAPI(Scope<Window>* window)
	{
		m_Window = window;
		GLSpriteRenderer();

		m_Texture = new GLTexture2D(std::filesystem::path("./assets/textures/cat.jpg"));
	}

	void OpenGLGraphicsAPI::Render()
	{
		EG_PROFILE_FUNCTION();
		m_Window->get()->GL_SwapWindow();

		GLSpriteRenderer::DrawSprite(m_Texture, glm::vec2(0, 0), glm::vec2(1, 1), 0, glm::vec3(255, 255, 255));
	}

	void OpenGLGraphicsAPI::SetVSync(bool value) {
		GLSpriteRenderer::SetVSync(value);
	}

	int OpenGLGraphicsAPI::GetVSync()
	{
		return GLSpriteRenderer::GetVSync();
	}
}
