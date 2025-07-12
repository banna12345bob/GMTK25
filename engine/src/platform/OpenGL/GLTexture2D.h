
#include <string>
#include <filesystem>

namespace Engine {
    // Handle to an image texture that exists in a gpu buffer.
    class GLTexture2D {
    public:
        // Id of the texture object
        unsigned int id;
        // Texture dimensions in pixels
        unsigned int width, height;
        // Name of texture (useful for debugging purposes)
        std::string name;

        // Generates the texture
        GLTexture2D(std::filesystem::path path);
        // Binds the texture as the current active GL_TEXTURE_2D texture object
        void Bind() const;
    };
}