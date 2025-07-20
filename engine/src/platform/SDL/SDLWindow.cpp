#include "SDLWindow.h"

#include <stb_image.h>

#include <filesystem>

#include <glad/glad.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

#include "engine/debug/Instrumentor.h"

namespace Engine {

	SDLWindow::SDLWindow(WindowProps props)
		: m_data(props)
	{
		EG_PROFILE_FUNCTION();
		uint32_t WindowFlags = SDL_WINDOW_OPENGL;
		WindowFlags |= SDL_WINDOW_RESIZABLE;

		EG_CORE_ASSERT(m_data.width > 0 && m_data.height > 0, "Invalid Window size");

		//Initialize SDL
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			EG_CORE_FATAL("SDL could not initialize! SDL_Error: {0}", SDL_GetError());
			EG_CORE_ASSERT(false, "SDL ERROR");
		}

		if (m_data.fullscreen) {
			int displaycount;
			const SDL_DisplayMode* DM = SDL_GetCurrentDisplayMode(SDL_GetDisplays(&displaycount)[0]);
			m_data.width = DM->w;
			m_data.height = DM->h;
		}

		m_window = SDL_CreateWindow(m_data.title.c_str(), m_data.width, m_data.height, WindowFlags);

		if (m_window == NULL)
		{
			EG_CORE_FATAL("Window could not be created! SDL_Error: {0}", SDL_GetError());
			EG_CORE_ASSERT(false, "SDL ERROR");
		}

		if (m_data.fullscreen)
			SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

		CreateGLContext();
		//switch (GraphicsAPI::GetAPI())
		//{
		//case GraphicsAPI::API::None: {
			//EG_CORE_ASSERT(false, "Cannot have GraphicsAPI::None");
			//break;
		//}
		//case GraphicsAPI::API::OpenGL: {
			//break;
		//}
		//}

		if (!std::filesystem::exists(m_data.pathToIcon)) {
			m_data.pathToIcon = "";
		}
		if (m_data.pathToIcon == "")
		{
			return;
		}

		// Probably could colapse into it's own createSurfaceFromFile function but I can't be bothered
		int width, height, bytesPerPixel;
		void* data = (void*)stbi_load(m_data.pathToIcon, &width, &height, &bytesPerPixel, 4);

		int pitch;
		pitch = width * bytesPerPixel;
		pitch = (pitch + 3) & ~3;

		uint32_t Rmask, Gmask, Bmask, Amask;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		Rmask = 0x000000FF;
		Gmask = 0x0000FF00;
		Bmask = 0x00FF0000;
		Amask = (bytesPerPixel == 4) ? 0xFF000000 : 0;
#else
		int s = (bytesPerPixel == 4) ? 0 : 8;
		Rmask = 0xFF000000 >> s;
		Gmask = 0x00FF0000 >> s;
		Bmask = 0x0000FF00 >> s;
		Amask = 0x000000FF >> s;
#endif

		SDL_Surface* icon = SDL_CreateSurfaceFrom(width, height, SDL_GetPixelFormatForMasks(bytesPerPixel * 8, Rmask, Gmask, Bmask, Amask), data, pitch);
		if (icon->pixels)
		{
			SDL_SetWindowIcon(m_window, icon);
		}
		else
		{
			EG_CORE_ASSERT(false, "Failed to load image");
		}
	}

	SDLWindow::~SDLWindow()
	{
		EG_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
		SDL_GL_DestroyContext(m_GLContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	void SDLWindow::SetWidth(int width)
	{
		EG_PROFILE_FUNCTION();
		if (width <= 0) {
			EG_CORE_WARN("Invalid window width: {0}", width);
			return;
		}

		m_data.width = width;
		SDL_SetWindowSize(m_window, m_data.width, m_data.height);
	}

	void SDLWindow::SetHeight(int height)
	{
		EG_PROFILE_FUNCTION();
		if (height <= 0) {
			EG_CORE_WARN("Invalid window height: {0}", height);
			return;
		}

		m_data.height = height;
		SDL_SetWindowSize(m_window, m_data.width, m_data.height);
	}

	void SDLWindow::ReloadWindow()
	{
		EG_PROFILE_FUNCTION();
		SDL_GetWindowSize(m_window, &m_data.width, &m_data.height);
	}

	/**
	* If turning VSync on, adaptive VSync will be tried first, then regualr VSync if that doesn't work.
	*/
	void SDLWindow::SetVSync(bool value) {
		if (value) {
			// Try adaptive VSync, if that doesn't work try the normal one
			if (!SDL_GL_SetSwapInterval(-1)) {
				EG_CORE_ERROR("Could not enable adaptive VSync: {0}", SDL_GetError());
				if (!SDL_GL_SetSwapInterval(1)) {
					EG_CORE_ERROR("Could not enable VSync: {0}", SDL_GetError());
				}
			}
		}
		else {
			if (!SDL_GL_SetSwapInterval(0)) {
				EG_CORE_ERROR("Could not disable VSync: {0}", SDL_GetError());
			}
		}

		int state;
		if (!SDL_GL_GetSwapInterval(&state)) {
			EG_CORE_ERROR("Could not get VSync state: {0}", SDL_GetError());
			return;
		}

		switch (state) {
		case 0:
			EG_CORE_INFO("VSync is off.");
			break;
		case 1:
			EG_CORE_INFO("VSync is on.");
			break;
		case -1:
			EG_CORE_INFO("Adaptive VSync is on.");
			break;
		}
	}

	/**
	* Returns current VSync mode
	* @returns 0 for off, 1 for VSync and -1 for adaptive VSync. Returns -2 on error
	*/
	int SDLWindow::GetVSync()
	{
		int state;
		if (!SDL_GL_GetSwapInterval(&state)) {
			EG_CORE_ERROR("Could not get VSync state: {0}", SDL_GetError());
			return -2;
		}
		return state;
	}

	void SDLWindow::CreateGLContext() 
	{
		EG_PROFILE_FUNCTION();
		// Create the OpenGl context
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		m_GLContext = SDL_GL_CreateContext(m_window);
		// Ensure the context was actually initialised
		if (m_GLContext == NULL) {
			EG_CORE_FATAL("SDL could not initialise the OpenGL context! {0}", SDL_GetError());
			EG_CORE_ASSERT(false, "SDL ERROR");
		}
		EG_CORE_INFO("Created openGL context");
		SDL_GL_MakeCurrent(m_window, m_GLContext);
		ImGui_ImplSDL3_InitForOpenGL(m_window, m_GLContext);
		ImGui_ImplOpenGL3_Init();

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			EG_CORE_FATAL("GLAD couldn't load OpenGL");
			EG_CORE_ASSERT(false, "GLAD ERROR");
		}
	}

	// Should NOT be in window
	void SDLWindow::GL_SwapWindow()
	{
		EG_PROFILE_FUNCTION();

		// Really should make ImGUI render independantly of the main window when undocked
		// TODO: Maybe multithread this or something idk
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		for (int i = 0; i < this->getImGuiLayersSize(); i++)
		{
			this->getImGuiLayer(i)->renderImGUILayer();
		}
		ImGui::Render();

		// TODO: Viewport stuff should NOT be here
		{
			// Updates the viewport
			glViewport(0, 0, this->GetWidth(), this->GetHeight());

			// Clears the viewport
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}
		SDL_GL_SwapWindow(m_window);
	}
}
