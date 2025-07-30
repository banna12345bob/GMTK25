#include "OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/core/Application.h"

namespace Engine {

	void OpenGLMessageCallback(
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


		glm::vec4 vertices[4] = {
		{ 0.5f,  0.5f, 0.0f, 1.0f },  // top right
		{ 0.5f, -0.5f, 0.0f, 1.0f },  // bottom right
		{ -0.5f, -0.5f, 0.0f, 1.0f },  // bottom left
		{ -0.5f,  0.5f, 0.0f, 1.0f }   // top left 
		};

		float vertices1[] = {
		-0.75f, 1.0f, 0.0f,  // top right
		-0.75f, 0.75f, 0.0f,  // bottom right
		-1.0f,  0.75f, 0.0f,  // bottom left
		-1.0f,  1.0f, 0.0f   // top left 
		};

		float vertices2[] = {
		0.75f, -1.0f, 0.0f,  // top right
		0.75f, -0.75f, 0.0f,  // bottom right
		1.0f,  -0.75f, 0.0f,  // bottom left
		1.0f,  -1.0f, 0.0f   // top left 
		};

		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		// ===================== Creates the vertex shader ===========================
		std::string vertexShaderSource = R"(
			#version 330 core
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
			#version 330 core
			in vec3 vPos;
			out vec4 FragColor;

			void main()
			{
			   FragColor = vec4(vPos + 0.5, 1.0f);
			}
			)";

		shader = new GLShader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

		// Generates VBO and EBO
		// VBO is your vertex buffer object
		// EBO is your element buffer object (your indicies)
		unsigned int VBO[5], EBO[5];
		glGenBuffers(5, VBO);
		glGenBuffers(5, EBO);

		// Generates the vertex array object
		// Basically all your attributes
		glGenVertexArrays(5, VAO);

		// Bind your VAO
		glBindVertexArray(VAO[0]);
		// Bind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		// Put the verticies into the VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Bind the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		// Put indices into EBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// How should OpenGL interprate the data
		// In this case it is an unnormalised vec3 float
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		// Enables the 0th vertex array attribute for this VAO
		glEnableVertexAttribArray(0);
		// Unbind the array
		glBindVertexArray(0);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), {0.75f, 0.75f, 0})
			* glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f });

		for (size_t i = 0; i < 4; i++)
		{
			vertices[i] = transform * vertices[i];
		}

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void OpenGLRenderAPI::Render()
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

		// Draw verticies using a VAO and our shader
		shader->Use();
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		Application::getApplication()->getImGuiRenderer()->EndFrame();

		SDL_Window* window = static_cast<SDL_Window*>(Application::getApplication()->getWindow()->getNativeWindow());
		SDL_GL_SwapWindow(window);
	}
}