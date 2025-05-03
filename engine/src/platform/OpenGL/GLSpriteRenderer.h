#include "engine/graphics/spriteRenderer.h"
#include <SDL3/SDL_video.h>
#include "engine/core/Window.h"

namespace Engine {
	class GLSpriteRenderer : public SpriteRenderer {
	public:
		GLSpriteRenderer();

		virtual void Render() override;
	};
}