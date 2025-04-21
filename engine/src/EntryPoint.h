#pragma once

#if defined(SS_PLATFORM_WINDOWS) || defined(SS_PLATFORM_MACOS_ARM64) || defined(SS_PLATFORM_MACOS_x86)

extern Engine::Application* Engine::CreateApplication();

	int main(int argc, char** argv)
	{
		Engine::Log::Init();

		auto app = Engine::CreateApplication();
		app->Run();
		delete app;
		return 0;
	}
#else
    #error "Entry point not built for current OS!"
#endif
