#include <engine.h>

class sandbox : public Engine::Application
{
public:

	sandbox()
	{

	}
	~sandbox()
	{
	}

	virtual void Run() {
		while (running)
			EG_INFO("Running");
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new sandbox();
}
