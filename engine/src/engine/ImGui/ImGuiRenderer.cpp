#include "ImGuiRenderer.h"

#include <imgui.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>


#include "engine/core/Application.h"

namespace Engine {

	ImGuiRenderer::ImGuiRenderer()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking		
		// Just be mindful that for some reason external ImGUI viewports are laggy when VSync is enabled
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		ImGui::StyleColorsDark();

		SDL_Window* window = static_cast<SDL_Window*>(Application::getApplication()->getWindow()->getNativeWindow());

		ImGui_ImplSDL3_InitForOpenGL(window, SDL_GL_GetCurrentContext());
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiRenderer::StartFrame()
	{
		EG_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		for (ImGuiLayer* layer : m_ImGuiLayers)
			layer->renderImGUILayer();
	}

	void ImGuiRenderer::EndFrame()
	{
		EG_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)Application::getApplication()->getWindow()->GetWidth(), (float)Application::getApplication()->getWindow()->GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		SDL_Window* window = static_cast<SDL_Window*>(Application::getApplication()->getWindow()->getNativeWindow());
		SDL_GL_SwapWindow(window);
	}

	void ImGuiRenderer::handleImGUIEvents(const SDL_Event* event)
	{
		ImGui_ImplSDL3_ProcessEvent(event);
	}

}