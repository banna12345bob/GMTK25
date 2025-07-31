#include "Renderer2D.h"

namespace Engine {

	void Renderer2D::Init()
	{

	}

	void Renderer2D::StartFrame() 
	{
		Application::getApplication()->getRenderAPI()->StartFrame();
	}

	void Renderer2D::EndFrame()
	{
		Application::getApplication()->getRenderAPI()->EndFrame();
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec3 scale, glm::vec3 colour)
	{
		Application::getApplication()->getRenderAPI()->RenderSquare(position, scale, colour);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec3 scale, std::string filepath, glm::vec3 tintColour)
	{
		EG_CORE_WARN("Needs to be implimented");
	}
}