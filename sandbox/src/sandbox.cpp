#include <engine.h>

#include "SandboxImGuiLayer.h"

class sandbox : public Engine::Application
{
public:

	sandbox(Engine::WindowProps props)
		: Engine::Application(props)
	{
		getImGuiRenderer()->registerImGuiLayer(new SandboxImGuiLayer());

		getCallbackManager()->registerKeyboardCallback(keyboardEventCallback);
		getCallbackManager()->registerMouseDownCallback(mouseDownEventCallback);

		m_sandBoxTexture = Engine::Texture2D::Create("assets/textures/Oak_Log.png");
		EG_TRACE("{0}, {1}", m_sandBoxTexture->getWidth(), m_sandBoxTexture->getHight());
	}

	~sandbox()
	{
	}

	virtual void UpdateApp() {
		// Run every frame
		Engine::Renderer2D::BeginScene();

		// GL_DEPTH_TEST is not enabled so be mindful of your drawing order
		Engine::Renderer2D::DrawQuad({ -0.75, 0.75, 0 }, { 0.25, 0.25 }, { 1, 0, 1, 1 });
		Engine::Renderer2D::DrawQuad({ 0, 0, -0.1 }, { 1, 1 }, m_sandBoxTexture, {1, 0, 0, 0.75});

		// Little test grid
		for (float x = -1.0f; x < 1.1f; x += 0.1f)
		{
			for (float y = 1; y > -1.1f; y -= 0.1f)
			{
				Engine::Renderer2D::DrawQuad({ x, y, 0 }, { 0.05, 0.05 }, { 0, 1, 1, 1 });
			}
		}

		Engine::Renderer2D::EndScene();
	}

	// A little example of how to do keyboard inputs
	static void keyboardEventCallback(void* callback) {
		EG_PROFILE_FUNCTION();
		if (Engine::Key::wasKeyPressed(EG_SCANCODE_I))
			Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/music/music1_short.wav", false, 0.4f);

		if (Engine::Key::wasKeyPressed(EG_SCANCODE_O))
			Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/music/John Coltrane - Naima.wav", true, 0.8f);
	}

	static void mouseDownEventCallback(void* callback) {
		if (Engine::Mouse::isButtonDown(1))
			EG_TRACE("Left mouse button pressed");
		if (Engine::Mouse::isButtonDown(2))
			EG_TRACE("Middle mouse button pressed");
		if (Engine::Mouse::isButtonDown(3))
			EG_TRACE("Right mouse button pressed");
	}

private:
	Engine::Ref<Engine::Texture2D> m_sandBoxTexture;
};

Engine::Application* Engine::CreateApplication()
{
	return new sandbox(Engine::WindowProps("Sandbox", 520, 520, "assets/textures/Oak_Log.png"));
}
