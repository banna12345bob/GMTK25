#include "engine/graphics/spriteRenderer.h"
#include <glad/glad.h>

namespace Engine {
	class GLSpriteRenderer : public SpriteRenderer {
	public:
		GLSpriteRenderer();

		virtual void Render() override;
	private:
		GLuint vao;
		GLuint shaderProgram;
	};
}