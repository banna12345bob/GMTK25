#include <glad/glad.h>

namespace Engine {
	class GLSpriteRenderer {
	public:
		GLSpriteRenderer();

		static void Render();
		static void SetVSync(bool value);
		static int GetVSync();
	private:
		inline static GLuint vao;
		inline static GLuint shaderProgram;
	};
}