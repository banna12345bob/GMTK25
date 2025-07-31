#pragma once

#include "engine/renderer/RenderAPI.h"


namespace Engine {

	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		OpenGLRenderAPI();

		virtual void StartFrame() override;
		virtual void EndFrame() override;
			
	};
}

