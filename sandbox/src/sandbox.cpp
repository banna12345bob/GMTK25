#include <engine.h>

class sandbox : public Engine::Application
{
public:

	sandbox(Engine::WindowProps props)
		: Engine::Application(props)
	{
		//m_AudioPlayer->PlaySound("assets/audio/music/John Coltrane - Naima.wav", true, 0.1f);
	}

	~sandbox()
	{
	}

	virtual void UpdateApp() {
		// Run every frame
	}

	// A little example of how to do keyboard inputs
	void keyboardEventCallback() {
		if (Engine::Key::wasKeyPressed(EG_SCANCODE_P)) {
			m_AudioPlayer->PlaySound("assets/audio/music/music1_short.wav", false, 0.4f, &musicID);
		}
		if (Engine::Key::wasKeyPressed(EG_SCANCODE_O))
		{
			m_AudioPlayer->StopSound(musicID);
		}
		if (Engine::Key::isKeyPressed(EG_SCANCODE_K))
		{
			m_Window->SetWidth(m_Window->GetWidth() + 10);
		}
		if (Engine::Key::isKeyPressed(EG_SCANCODE_L))
		{
			m_Window->SetWidth(m_Window->GetWidth() - 10);
		}
		if (Engine::Key::wasKeyPressed(EG_SCANCODE_F)) {
			m_showFPS = !m_showFPS;
		}
		if (Engine::Key::wasKeyPressed(EG_SCANCODE_V)) {
			m_GraphicsAPI->SetVSync((m_GraphicsAPI->GetVSync() == 0));
		}
	}
	
private:
	unsigned int musicID;
};

sandbox* app;

// This sucks ass
// If I include registerKeyboardCallback in the constructor of sandbox it complains that the event callback isn't static
// When I make the callback static then it can't access memeber variables
// So I have to make a global callback wrapper that can then get parsed into the register
// ¯\_(ツ)_/¯ - Isaac

static void keyboardEventCallbackWrapper(void* prt) {
	app->keyboardEventCallback();
}

Engine::Application* Engine::CreateApplication()
{
	app = new sandbox(Engine::WindowProps("Sandbox", 520, 520, "assets/textures/Oak_Log.png"));
	app->m_EventCallbackManager->registerKeyboardCallback(&keyboardEventCallbackWrapper);
	return app;
}
