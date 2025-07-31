#include "OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/core/Application.h"
#include "engine/renderer/VertexArray.h"
#include "engine/renderer/VertexBuffer.h"

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


		// ===================== Creates the vertex shader ===========================
		std::string vertexShaderSource = R"(
			#version 440 core
			layout (location = 0) in vec3 aPos;
			out vec3 vPos;

			void main()
			{
				gl_Position = vec4(aPos, 1.0);
				vPos = aPos;
			}
			)";

		// ================== Creates the fragment shader ============================
		std::string fragmentShaderSource = R"(
			#version 440 core
			in vec3 vPos;
			out vec4 FragColor;

			uniform vec3 fColour;

			void main()
			{
				FragColor = vec4(fColour, 1.0f);
			}
			)";

		shader = new GLShader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
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

	void OpenGLRenderAPI::RenderSquare(glm::vec3 position, glm::vec3 scale, glm::vec3 colour)
	{
		// Developer we have a memory leak
		// I think its' the fact that we're recreating the buffers every frame

		glm::vec4 newVerts[4];

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), scale);

		for (size_t i = 0; i < 4; i++)
		{
			newVerts[i] = transform * vertices[i];
		}

		// Generates VBO and EBO
		// VBO is your vertex buffer object
		// EBO is your element buffer object (your indicies)
		unsigned int EBO;

		glGenBuffers(1, &EBO);

		Ref<VertexArray> vertexArray = VertexArray::Create();

		vertexArray->Bind();
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(newVerts, sizeof(newVerts));

		// Bind the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// Put indices into EBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// How should OpenGL interprate the data
		// In this case it is an unnormalised vec3 float
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		// Enables the 0th vertex array attribute for this VAO
		glEnableVertexAttribArray(0);
		// Unbind the array
		glBindVertexArray(0);


		// Draw verticies using a VAO and our shader
		shader->Use();
		shader->SetVector3f("fColour", colour);
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}