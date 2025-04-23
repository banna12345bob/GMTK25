#include <engine.h>

#include <SDL3/SDL_scancode.h>

class sandbox : public Engine::Application
{
public:

	sandbox(Engine::WindowProps props)
		: Engine::Application(props)
	{
		//m_AudioPlayer->PlaySound("assets/audio/music/music1_short.wav", false, 0.4f, NULL);
		m_EventCallbackManager->registerKeyboardCallback(&keyboardEventCallback);
	}
	~sandbox()
	{
	}

	virtual void UpdateApp() {
		// Run every frame
		//EG_INFO("Running");
	}

	static void keyboardEventCallback(SDL_Scancode key) {
		switch (key)
		{
		case SDL_SCANCODE_K:
			EG_INFO("K key pressed");
			break;
		case SDL_SCANCODE_L:
			EG_INFO("L key pressed");
			break;
		}
	}

};

Engine::Application* Engine::CreateApplication()
{
	return new sandbox(Engine::WindowProps("Sandbox", 520, 520, "assets/textures/Oak_Log.png"));
}
