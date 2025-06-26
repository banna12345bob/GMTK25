#include "GLSpriteRenderer.h"

#include "engine/core/core.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <stb_image.h>

#include "engine/debug/Instrumentor.h"

namespace Engine {
	GLSpriteRenderer::GLSpriteRenderer() {
		EG_PROFILE_FUNCTION();
		// Initialize GLAD
		// This loads all of the functions from the GPU's OpenGL drivers
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			EG_CORE_FATAL("GLAD couldn't load OpenGL");
			EG_CORE_ASSERT(false, "GLAD ERROR");
		}

		const unsigned char* vendor = glGetString(GL_VENDOR);
		const unsigned char* renderer = glGetString(GL_RENDERER);
		EG_CORE_INFO("GLAD loaded OpenGL sucessfully! {0}, {1}", (char*)(vendor), (char*)(renderer));

		// Source code for vertex shader
		const GLchar* vertexSource =
			"#version 330 core\n"
			"layout (location = 0) in vec3 aPos;"
			"layout (location = 1) in vec2 aTexCoord;"
			"out vec2 texCoord;"
			"void main() {"
			"   texCoord = aTexCoord;"
			"	gl_Position = vec4(aPos, 1.0);"
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
			"in vec2 texCoord;"
			"out vec4 FragColor;"
			"uniform sampler2D sampler;"
			"void main() {"
			"	FragColor = texture(sampler, texCoord);"
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
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		// Enable vertex attribute with its location (zero)
		// Needed because vertex attributes are disabled by default
		//glEnableVertexAttribArray(0);

		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
		//glEnableVertexAttribArray(1);

		float vertices[] = { 0.9f,  0.9f, 0.0f, 0.0f, 0.0f,   // top right
	                         0.9f, -0.9f, 0.0f, 0.0f, 3.0f,   // bottom right
	                        -0.9f, -0.9f, 0.0f, 3.0f, 3.0f,   // bottom left
	                        -0.9f,  0.9f, 0.0f, 3.0f, 0.0f,   // top left 
		};
		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3,
		};

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenTextures(1, &texture);
		// Bind the texture to OpenGL's 2d texture slot
		glBindTexture(GL_TEXTURE_2D, texture);
		// The following functions operate on our texture, since it is bound to OpenGL's 2d texture slot
		// Set texture wrapping (this doesn't matter for our use case, so we'll just set it to repeat)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		// Set texture filtering
		// Since this game will likely be pixel art, set to nearest neighbour
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Generate our texture
		// Width, height, and # of colour channels of our image
		int width, height, colourChannels;
		// The desired amount of channels for our image
		// For some reason, OpenGL does not like working with RGB images, so setting this to 4 will ensure RGB images are converted to RGBA
		const int DESIRED_CHANNELS = 4;
		unsigned char* textureData = stbi_load("./assets/textures/cat.jpg", &width, &height, &colourChannels, DESIRED_CHANNELS);

		if (!textureData) {
			const char* failure_reason = stbi_failure_reason();
			EG_CORE_FATAL("Failed to load texture! {0}", failure_reason);
			EG_CORE_ASSERT(false, "STB Image Error");
		}

		EG_CORE_INFO("{0}, {1}, {2}", width, height, colourChannels);

		// Generate the texture buffer on the GPU
		// Parameters:
		// - texture target
		// - mipmap level of the generated texture
		// - what format the texture should be stored in
		// - texture width
		// - texture height
		// - format of the source image
		// - data type of the source image (ours is stored in `char`s)
		// - actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
		// glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, textureData);

		// Free the original image data since it isn't needed anymore
		stbi_image_free(textureData);
	}

	void GLSpriteRenderer::Render() {
		EG_PROFILE_FUNCTION();
		// Clear screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
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