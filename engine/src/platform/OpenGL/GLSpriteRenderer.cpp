#include "GLSpriteRenderer.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>

namespace Engine {
	GLSpriteRenderer::GLSpriteRenderer() {
		// Initialize GLAD
		// This loads all of the functions from the GPU's OpenGL drivers
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			EG_CORE_FATAL("GLAD couldn't load OpenGL");
			EG_CORE_ASSERT(false, "GLAD ERROR");
		}

		// Source code for vertex shader
		const GLchar* vertexSource =
			"#version 330 core\n"
			"layout (location = 0) in vec3 aPos;"
			"void main() {"
			"	gl_Position = vec4(aPos.xyz, 1.0);"
			"}";

		// Create vertex shader object
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Attach shader source code to shader object
		glShaderSource(vertexShader, 1, &vertexSource, NULL);

		// Compile vertex shader
		glCompileShader(vertexShader);
		// Ensure shader compilation succeeded
		int success;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			EG_CORE_FATAL("Vertex shader compilation failed! {0}", infoLog);
			EG_CORE_ASSERT(false, "OpenGL Error");
		}

		// Source code for the fragment shader
		const GLchar* fragmentSource =
			"#version 330 core\n"
			"out vec4 FragColor;"
			"void main() {"
			"	FragColor = vec4(1.0, 0.5, 0.2, 1.0);"
			"}";

		// Create fragment shader object
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Attach shader source code to shader object
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

		// Compile fragment shader
		glCompileShader(fragmentShader);
		// Ensure shader compilation succeeded
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			EG_CORE_FATAL("Fragment shader compilation failed! {0}", infoLog);
			EG_CORE_ASSERT(false, "OpenGL Error");
		}

		// Link vertex and fragment shaders into a shader program
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Ensure shader program linking succeeded
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			EG_CORE_FATAL("Shader program linking failed! {0}", infoLog);
			EG_CORE_ASSERT(false, "OpenGL Error");
		}

		// Use this program for all subsequent render calls
		glUseProgram(shaderProgram);

		// We don't need the shaders anymore, delete them
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Specify vertex data layout
		// Parameters:
		// - which vertex attribute to configure (we specified position attrib at (location = 0)
		// - size of the vertex attribute (its a vec3, so its composed of 3 values)
		// - type of data, which is GL_FLOAT
		// - set true if we want data to be normalized, its not relevant to us so we set it to false
		// - stride, which is the space between consecutive vertex attributes
		// - offset, because we dont care, we do a weird void* cast
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		// Enable vertex attribute with its location (zero)
		// Needed because vertex attributes are disabled by default
		glEnableVertexAttribArray(0);

		float vertices[] = { -0.5f, -0.5f, 0.0f, 
							  0.5f, -0.5f, 0.0f,
							  0.0f,  0.5f, 0.0f };

		/*
		// Generate vertex buffer
		GLuint vbo;
		glGenBuffers(1, &vbo);

		// Bind buffer object
		// From now on, any buffer calls we make on the GL_ARRAY_BUFFER target will be used to configure the current bound buffer (vbo)
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// Copy vertex data into buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		*/

		// Create a Vertex Array Object (VAO)
		// Core OpenGL requires that we use a VAO so it knows what to do with our vertex inputs
		// If we fail to bind a VAO, OpenGL will likely refuse to draw anything

		// Generate the VAO
		glGenVertexArrays(1, &vao);
		// Bind VAO
		glBindVertexArray(vao);
		// Copy vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, vao);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Set vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}

	void GLSpriteRenderer::Render() {
		// Clear screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}