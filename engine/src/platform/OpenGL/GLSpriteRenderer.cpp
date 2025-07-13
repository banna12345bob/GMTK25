#include "GLSpriteRenderer.h"

#include "engine/core/core.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <filesystem>

#include "engine/debug/Instrumentor.h"

namespace Engine {
	GLSpriteRenderer::GLSpriteRenderer() {
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			EG_CORE_FATAL("GLAD couldn't load OpenGL");
			EG_CORE_ASSERT(false, "GLAD ERROR");
		}

		const char* vertexShader = R"(
	    #version 330 core
	    layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

		out vec2 TexCoords;

		uniform mat4 model;
		uniform mat4 projection;

		void main()
		{
			TexCoords = vertex.zw;
			gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
		})";

		const char* fragmentShader = R"(
		#version 330 core
		in vec2 TexCoords;
		out vec4 color;

		uniform sampler2D image;
		uniform vec3 spriteColor;

		void main()
		{
			color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
		})";

		shader = new GLShader(vertexShader, fragmentShader);

		glGenVertexArrays(1, &vao);

		// ** create and fill vbo ** //

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		float vertices[] = {
			// pos      // tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(vao);

		// ** configure vertex attributes ** //

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		// ** finally, bind vbo and vao ** //

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);
	}

	void GLSpriteRenderer::DrawSprite(GLTexture2D* texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
		shader->Use();

		// prepare transformations
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

		model = glm::scale(model, glm::vec3(size, 1.0f));

		shader->SetMatrix4("model", model);
		shader->SetVector3f("spriteColor", color);

		glActiveTexture(GL_TEXTURE0);
		texture->Bind();

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	/**
	* If turning VSync on, adaptive VSync will be tried first, then regualr VSync if that doesn't work.
	*/
	void GLSpriteRenderer::SetVSync(bool value) {
		if (value) {
			// Try adaptive VSync, if that doesn't work try the normal one
			if (!SDL_GL_SetSwapInterval(-1)) {
				EG_CORE_ERROR("Could not enable adaptive VSync: {0}", SDL_GetError());
				if (!SDL_GL_SetSwapInterval(1)) {
					EG_CORE_ERROR("Could not enable VSync: {0}", SDL_GetError());
				}
			}
		}
		else {
			if (!SDL_GL_SetSwapInterval(0)) {
				EG_CORE_ERROR("Could not disable VSync: {0}", SDL_GetError());
			}
		}

		int state;
		if (!SDL_GL_GetSwapInterval(&state)) {
			EG_CORE_ERROR("Could not get VSync state: {0}", SDL_GetError());
			return;
		}

		switch (state) {
		case 0:
			EG_CORE_INFO("VSync is off.");
			break;
		case 1:
			EG_CORE_INFO("VSync is on.");
			break;
		case -1:
			EG_CORE_INFO("Adaptive VSync is on.");
			break;
		}
	}

	/**
	* Returns current VSync mode
	* @returns 0 for off, 1 for VSync and -1 for adaptive VSync. Returns -2 on error
	*/
	int GLSpriteRenderer::GetVSync()
	{
		int state;
		if (!SDL_GL_GetSwapInterval(&state)) {
			EG_CORE_ERROR("Could not get VSync state: {0}", SDL_GetError());
			return -2;
		}
		return state;
	}
}