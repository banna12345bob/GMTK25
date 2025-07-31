#include "Application.h"

#include "engine/debug/Instrumentor.h"

#include "engine/core/Keycodes.h"
#include "engine/events/Key.h"

#include "engine/renderer/Renderer2D.h"

namespace Engine {

	static Application* s_Instance = nullptr;

	Application::Application(WindowProps props)
	{
		EG_PROFILE_FUNCTION();
		s_Instance = this;

		m_Window = Window::Create(props);
		m_RenderAPI = RenderAPI::Create();
		m_EventCallbackManager = new eventCallbackManager();
		m_AudioPlayer = AudioPlayer::Create();
		m_ImGuiRenderer = new ImGuiRenderer();

		m_Window->SetVSync(false);

		getImGuiRenderer()->registerImGuiLayer(m_AudioDebuggerLayer);

		m_EventCallbackManager->registerKeyboardCallback(AudioDebuggerKeyboardEventCallback);
	}

	Application::~Application()
	{
		EG_PROFILE_FUNCTION();
		Renderer2D::Shutdown();
	}

	Application* Engine::Application::getApplication()
	{
		return s_Instance;
	}

	void Application::AudioDebuggerKeyboardEventCallback(void* callback) {
		EG_PROFILE_FUNCTION();
		if ((Key::isKeyPressed(EG_SCANCODE_LCTRL) || Key::isKeyPressed(EG_SCANCODE_RCTRL)) && Key::wasKeyPressed(EG_SCANCODE_P))
			Application::getApplication()->m_AudioDebuggerLayer->m_ShowWindow = !Application::getApplication()->m_AudioDebuggerLayer->m_ShowWindow;

		if (Key::wasKeyPressed(EG_SCANCODE_F1)) {
			if (Application::getApplication()->m_RenderAPI->getRenderMode() == RenderAPI::RenderMode::Normal)
				Application::getApplication()->m_RenderAPI->setRenderMode(RenderAPI::RenderMode::Wireframe);
			else
				Application::getApplication()->m_RenderAPI->setRenderMode(RenderAPI::RenderMode::Normal);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		EG_PROFILE_FUNCTION();
		int a, b, deltaTime;
		b = 0;
		Renderer2D::Init();

		while (m_Window->GetRunning()) {
			a = (int)SDL_GetTicks();
			deltaTime = a - b;

			if (deltaTime != 0)
				m_frameRate = 1000 / deltaTime;

			m_Window->HandleEvents();

			m_RenderAPI->SetClearColor({ 0, 0, 0, 0 });
			m_RenderAPI->Clear();
			for (Layer* layer : m_layerStack)
				layer->OnUpdate();

			m_ImGuiRenderer->StartFrame();
			for (Layer* layer : m_layerStack)
				layer->OnImGuiRender();
			m_AudioDebuggerLayer->renderImGUILayer();
			m_ImGuiRenderer->EndFrame();

			SDL_Window* window = static_cast<SDL_Window*>(Application::getApplication()->getWindow()->getNativeWindow());
			SDL_GL_SwapWindow(window);

			b = a;
		}
	}
}
