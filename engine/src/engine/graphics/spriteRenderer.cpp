#include "spriteRenderer.h"

#include "platform/OpenGL/GLSpriteRenderer.h"

namespace Engine {

	Scope<SpriteRenderer> SpriteRenderer::Create() 
	{
		return CreateScope<GLSpriteRenderer>();
	}

}