#include "SDLWindow.h"

#include "engine/graphics/GraphicsAPI.h"

#include <stb_image.h>

#include <filesystem>

#include <SDL3/SDL_opengles2.h>

#include <glad/glad.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>

namespace Engine {

	SDLWindow::SDLWindow(WindowProps props)
		: m_data(props)
	{
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

		switch (GraphicsAPI::GetAPI())
		{
		case GraphicsAPI::API::None: EG_CORE_ASSERT(false, "Cannot have GraphicsAPI::None");
		case GraphicsAPI::API::OpenGL: CreateGLContext();
		}
	}

	SDLWindow::~SDLWindow()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	void SDLWindow::SetWidth(int width)
	{
		if (width <= 0) {
			EG_CORE_WARN("Invalid window width: {0}", width);
			return;
		}

		m_data.width = width;
		SDL_SetWindowSize(m_window, m_data.width, m_data.height);
	}

	void SDLWindow::SetHeight(int height)
	{
		if (height <= 0) {
			EG_CORE_WARN("Invalid window height: {0}", height);
			return;
		}

		m_data.height = height;
		SDL_SetWindowSize(m_window, m_data.width, m_data.height);
	}

	void SDLWindow::ReloadWindow()
	{
		SDL_GetWindowSize(m_window, &m_data.width, &m_data.height);
	}

	void SDLWindow::CreateGLContext() 
	{
		// Create the OpenGl context
		SDL_GLContext sdl_gl_ctx = SDL_GL_CreateContext(m_window);
		// Ensure the context was actually initialised
		if (sdl_gl_ctx == NULL) {
			EG_CORE_FATAL("SDL could not initialise the OpenGL context! {0}", SDL_GetError());
			EG_CORE_ASSERT(false, "SDL ERROR");
		}
		SDL_GL_MakeCurrent(m_window, sdl_gl_ctx);
		ImGui_ImplSDL3_InitForOpenGL(m_window, sdl_gl_ctx);
		ImGui_ImplOpenGL3_Init();
	}

	void SDLWindow::GL_SwapWindow()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::Render();
		// Updates the window
		glViewport(0, 0, this->GetWidth(), this->GetHeight());
		SDL_GL_SwapWindow(m_window);
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
