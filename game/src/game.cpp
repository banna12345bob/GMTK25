#include <engine.h>
#include "gameplay/Block.h"


class game : public Engine::Application
{
public:

	game(Engine::WindowProps props)
		: Engine::Application(props)
	{
		getCallbackManager()->registerKeyboardCallback(keyboardEventCallback);
		getCallbackManager()->registerMouseDownCallback(mouseDownEventCallback);

		game1::Block::LoadBlockData();
	}

	~game()
	{
	}

	virtual void UpdateApp() {
		// Run every frame

		// GL_DEPTH_TEST is not enabled so be mindful of your drawing order
		Engine::Renderer2D::DrawQuad({ 0, 0, 0 }, { 1, 1, 1 }, { 1, 1, 1 });
		Engine::Renderer2D::DrawQuad({ -0.75, 0.75, 0.5 }, { 0.25, 0.25, 1 }, { 1, 0, 1 });

		// Little test grid
		/*for (float x = -1.0f; x < 1.0f; x += 0.1f)
		{
			for (float y = 1; y > -1.0f; y -= 0.1f)
			{
				Engine::Renderer2D::DrawQuad({ x, y, 0.5 }, { 0.05, 0.05, 1 }, { 0, 1, 1 });
			}
		}*/
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
	return new game(Engine::WindowProps("Sandbox", 520, 520, "assets/textures/Oak_Log.png"));
}
