#include <glad/glad.h>

namespace Engine {
	class GLSpriteRenderer {
	public:
		GLSpriteRenderer();

		void Render();
		int SetVSync(bool value);
	private:
		GLuint vao;
		GLuint shaderProgram;
	};
}