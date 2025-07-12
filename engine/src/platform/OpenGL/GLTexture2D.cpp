#include "GLTexture2D.h"
#include "engine/core/core.h"

#include "glad/glad.h"
#include <SDL3/SDL.h>
#include <stb_image.h>

#include <string>
#include <filesystem>

#include "engine/debug/Instrumentor.h"

namespace Engine {
	GLTexture2D::GLTexture2D(std::filesystem::path path) {
		// ** Generate our texture ** //

		glGenTextures(1, &id);

		// Width, height, and # of colour channels of our image
		int texWidth, texHeight, texColourChannels;
		// The desired amount of channels for our image
		// For some reason, OpenGL does not like working with RGB images, so setting this to 4 will ensure RGB images are converted to RGBA
		const int DESIRED_CHANNELS = 4;
		unsigned char* textureData = stbi_load(path.string().c_str(), &texWidth, &texHeight, &texColourChannels, DESIRED_CHANNELS);

		width = texWidth;
		height = texHeight;

		// Use the file name as the image texture's name.
		std::string fileStem = path.stem().string();
		name = fileStem.substr(0, fileStem.find_last_of('.'));

		if (!textureData) {
			// Unable to load the provided texture for some reason
			const char* failure_reason = stbi_failure_reason();
			EG_CORE_FATAL("Failed to load image {0}! {1}", path.string(), failure_reason);
			EG_CORE_ASSERT(false, "STB Image Error");
		}

		// Bind the texture to OpenGL's 2d texture slot
		glBindTexture(GL_TEXTURE_2D, id);

		// ** The following functions will now operate on our texture, since it is bound to OpenGL's 2d texture slot ** //

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

		// Set texture wrapping (this doesn't matter for our use case, so we'll just set it to repeat)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		// Set texture filtering
		// Since this game will likely be pixel art, set to nearest neighbour
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Free the original image data since it isn't needed anymore
		stbi_image_free(textureData);

		EG_CORE_INFO("Created Image Texture {{ name: {0}, width: {1}, height: {2} }}", name, width, height);
	}

	void GLTexture2D::Bind() const {
		glBindTexture(GL_TEXTURE_2D, this->id);
	}
}
