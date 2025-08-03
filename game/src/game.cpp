#include <engine.h>
#include "gameplay/Block.h"
#include "GameLayer.h"

#include <engine/core/EntryPoint.h>


class game : public Engine::Application
{
public:

	game(Engine::WindowProps props)
		: Engine::Application(props)
	{
		PushLayer(new GameLayer());

		// miniz_cpp::zip_file file;
		// file.load("assets/assets.zip");
		// EG_TRACE(file.read("shaders/colour.glsl"));
	}

	~game()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new game(Engine::WindowProps("Game", 900, 900, "assets/textures/Oak_Log.png"));
}
