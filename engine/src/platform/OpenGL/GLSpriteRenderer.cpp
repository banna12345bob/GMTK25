#include "GLSpriteRenderer.h"

#include <glad/glad.h>

namespace Engine {
	GLSpriteRenderer::GLSpriteRenderer() {
		float vertices[] = { 0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f };

		GLuint vbo;
		glGenBuffers(1, &vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Create and compile vertex shader

		// Source code for the shader
		const GLchar* vertexSource =
			"attribute vec4 position"
			"void main() {"
			"	gl_Position = vec4(position.xyz, 1.0);"
			"}";

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		// Create and compile vertex shader

		// Source code for the fragment shader
		const GLchar* fragmentSource =
			"precision mediump float;"
			"void main() {"
			"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
			"}";

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);

		// Link vertex and fragment shaders into a shader program
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);

		// Specify vertex data layout
		GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void GLSpriteRenderer::Render() {
		// Clear screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}