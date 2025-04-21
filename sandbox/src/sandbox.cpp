#include <engine.h>

class sandbox : public Engine::Application
{
public:

	sandbox(Engine::WindowProps props)
		: Engine::Application(props)
	{

	}
	~sandbox()
	{
	}

	virtual void UpdateApp() {
		// Run every frame
		//EG_INFO("Running");
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new sandbox(Engine::WindowProps("Engine", 520, 520, "assets/textures/Oak_Log.png"));
}
