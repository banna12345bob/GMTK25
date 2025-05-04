#include "engine/graphics/spriteRenderer.h"

namespace Engine {
	class GLSpriteRenderer : public SpriteRenderer {
	public:
		GLSpriteRenderer();

		virtual void Render() override;
	};
}