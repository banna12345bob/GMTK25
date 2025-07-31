#include "OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/core/Application.h"

namespace Engine {

	static void OpenGLMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         EG_CORE_FATAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       EG_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          EG_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: EG_CORE_TRACE(message); return;
		}

		EG_CORE_FATAL("Unknown severity level!");
	}

	OpenGLRenderAPI::OpenGLRenderAPI()
	{
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			EG_CORE_FATAL("GLAD couldn't load OpenGL");
			EG_CORE_ASSERT(false, "GLAD ERROR");
		}

#ifdef EG_DEBUG

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

#endif // EG_DEBUG

		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRenderAPI::StartFrame()
	{
		EG_PROFILE_FUNCTION();

		Application::getApplication()->getImGuiRenderer()->StartFrame();

		// Updates the viewport
		glViewport(0, 0, Application::getApplication()->getWindow()->GetWidth(), Application::getApplication()->getWindow()->GetHeight());

		// Clears the viewport
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		switch (getRenderMode()) {
		case RenderMode::Normal:    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
		case RenderMode::Wireframe: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
		}

	}

	void OpenGLRenderAPI::EndFrame() 
	{
		Application::getApplication()->getImGuiRenderer()->EndFrame();

		SDL_Window* window = static_cast<SDL_Window*>(Application::getApplication()->getWindow()->getNativeWindow());
		SDL_GL_SwapWindow(window);
	}
}