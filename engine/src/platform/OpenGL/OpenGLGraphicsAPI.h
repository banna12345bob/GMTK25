#pragma once
#include "engine/graphics/GraphicsAPI.h"

#include "GLSpriteRenderer.h"

namespace Engine {

	class OpenGLGraphicsAPI : public GraphicsAPI
	{
	public:
		OpenGLGraphicsAPI(Scope<Window>* window);
		virtual void Render() override;

		virtual void SetVSync(bool value) override;
		virtual int GetVSync() override;

	private:
		Scope<Window>* m_Window;

		GLTexture2D* m_Texture;
	};
}

