#include <engine.h>

#include <SDL3/SDL_scancode.h>

class sandbox : public Engine::Application
{
public:

	sandbox(Engine::WindowProps props)
		: Engine::Application(props)
	{
		//m_AudioPlayer->PlaySound("assets/audio/music/music1_short.wav", false, 0.4f, NULL);
	}
	~sandbox()
	{
	}

	virtual void UpdateApp() {
		// Run every frame
		//EG_INFO("Running");
	}

	void keyboardEventCallback(SDL_Scancode key) {
		switch (key)
		{
		case SDL_SCANCODE_K:
			m_Window->SetWidth(m_Window->GetWidth() + 10);
			break;
		case SDL_SCANCODE_L:
			m_Window->SetWidth(m_Window->GetWidth() - 10);
			break;
		}
	}

};

sandbox* app;

// This sucks ass
// If I include registerKeyboardCallback in the constructor of sandbox it complains that the event callback isn't static
// When I make the callback static then it can't access memeber variables
// So I have to make a global callback wrapper that can then get parsed into the register
// ¯\_(ツ)_/¯ - Isaac

void keyboardEventCallbackWrapper(SDL_Scancode key) {
	app->keyboardEventCallback(key);
}

Engine::Application* Engine::CreateApplication()
{
	app = new sandbox(Engine::WindowProps("Sandbox", 520, 520, "assets/textures/Oak_Log.png"));
	app->m_EventCallbackManager->registerKeyboardCallback(&keyboardEventCallbackWrapper);
	return app;
}
