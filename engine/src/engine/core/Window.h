#pragma once
#include <string>

#include "engine/core/core.h"

#include "engine/graphics/imgui/ImGUILayer.h"

namespace Engine {

	struct WindowProps
	{
		std::string title;
		int width;
		int height;
		const char* pathToIcon;
		bool fullscreen;

		//Default constructor
		WindowProps(const std::string& title,
			int width,
			int height,
			const char* pathToIcon = "",
			bool fullscreen = false)
			: title(title), width(width), height(height), pathToIcon(pathToIcon), fullscreen(fullscreen)
		{
		}
	};

	class Window
	{
	public:
		virtual ~Window() {}

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		virtual void SetWidth(int width) = 0;
		virtual void SetHeight(int height) = 0;

		virtual void ReloadWindow() = 0;

		/**
		* If turning VSync on, adaptive VSync will be tried first, then regualr VSync if that doesn't work.
		*/
		virtual void SetVSync(bool value) = 0;
		/**
		* Returns current VSync mode
		* @returns 0 for off, 1 for VSync and -1 for adaptive VSync. Returns -2 on error
		*/
		virtual int GetVSync() = 0;

		virtual bool GetRunning() { return m_Running; }
		virtual void SetRunning(bool running) { m_Running = running; }
		
		virtual void GL_SwapWindow() {}

		static Scope<Window> Create(WindowProps props);

		void registerImGuiLayer(ImGuiLayer* ImGuiLayer) { m_ImGuiLayers.push_back(ImGuiLayer); }
		ImGuiLayer* getImGuiLayer(int index) { return m_ImGuiLayers[index]; }
		int getImGuiLayersSize() { return (int)m_ImGuiLayers.size(); }
	private:
		bool m_Running = true;

		std::vector<ImGuiLayer*> m_ImGuiLayers;
	};
}

