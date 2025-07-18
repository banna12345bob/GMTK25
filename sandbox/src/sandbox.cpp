#include <engine.h>

#include "SandboxImGuiLayer.h"

class sandbox : public Engine::Application
{
public:

	sandbox(Engine::WindowProps props)
		: Engine::Application(props)
	{
		m_Window->registerImGuiLayer(new SandboxImGuiLayer());

		m_EventCallbackManager->registerKeyboardCallback(keyboardEventCallback);
	}

	~sandbox()
	{
	}

	virtual void UpdateApp() {
		// Run every frame
	}

	// A little example of how to do keyboard inputs
	static void keyboardEventCallback(void* callback) {
		EG_PROFILE_FUNCTION();
		if (Engine::Key::wasKeyPressed(EG_SCANCODE_I))
			Engine::Application::getApplication()->m_AudioPlayer->PlaySound("assets/audio/music/music1_short.wav", false, 0.4f);

		if (Engine::Key::wasKeyPressed(EG_SCANCODE_O))
			Engine::Application::getApplication()->m_AudioPlayer->PlaySound("assets/audio/music/John Coltrane - Naima.wav", true, 0.8f);
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new sandbox(Engine::WindowProps("Sandbox", 520, 520, "assets/textures/Oak_Log.png"));
}
