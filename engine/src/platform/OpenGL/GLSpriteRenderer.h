#include <glad/glad.h>

namespace Engine {
	class GLSpriteRenderer {
	public:
		GLSpriteRenderer();

		void Render();
	private:
		GLuint vao;
		GLuint shaderProgram;
	};
}