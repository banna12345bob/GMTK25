#include <engine.h>
#include "gameplay/Block.h"
#include "GameLayer.h"

#include <engine/core/EntryPoint.h>

#include <zip_file.hpp>

class game : public Engine::Application
{
public:

	game(Engine::WindowProps props)
		: Engine::Application(props)
	{
		getCallbackManager()->registerKeyboardCallback(keyboardEventCallback);
		getCallbackManager()->registerMouseDownCallback(mouseDownEventCallback);

		PushLayer(new GameLayer());

		// miniz_cpp::zip_file file;
		// file.load("assets/assets.zip");
		// EG_TRACE(file.read("shaders/colour.glsl"));
	}

	~game()
	{
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
		if (Engine::Mouse::isButtonDown(EG_MOUSECODE_LEFT))
			EG_TRACE("Left mouse button pressed");
		if (Engine::Mouse::isButtonDown(EG_MOUSECODE_MIDDLE))
			EG_TRACE("Middle mouse button pressed");
		if (Engine::Mouse::isButtonDown(EG_MOUSECODE_RIGHT))
			EG_TRACE("Right mouse button pressed");
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new game(Engine::WindowProps("Game", 900, 900, "assets/textures/Oak_Log.png"));
}
