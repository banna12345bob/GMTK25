#include "GLFWWindow.h"

#include "engine/debug/Instrumentor.h"
#include "engine/renderer/RenderCommand.h"

namespace Engine {

	static void GLFWErrorCallback(int error, const char* description)
	{
		EG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	GLFWWindow::GLFWWindow(WindowProps props)
		: m_data(props)
	{
		Init(props);
	}

	void GLFWWindow::Init(WindowProps props)
	{
		EG_PROFILE_FUNCTION();

		EG_CORE_ASSERT(m_data.width > 0 && m_data.height > 0, "Invalid Window size");
		EG_CORE_INFO("Creating GLFW window {0} ({1}, {2})", m_data.title, m_data.width, m_data.height);

		int success = glfwInit();
		EG_CORE_ASSERT(success, "Could not intialise GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(m_window);
	}

	void GLFWWindow::SetWidth(int width)
	{
		if (width <= 0) {
			EG_CORE_WARN("Invalid window width: {0}", width);
			return;
		}

		m_data.width = width;
		glfwSetWindowSize(m_window, m_data.width, m_data.height);
	}

	void GLFWWindow::SetHeight(int height)
	{
		if (height <= 0) {
			EG_CORE_WARN("Invalid window height: {0}", height);
			return;
		}

		m_data.height = height;
		glfwSetWindowSize(m_window, m_data.width, m_data.height);
	}

	void GLFWWindow::ReloadWindow()
	{
		glfwGetWindowSize(m_window, &m_data.width, &m_data.height);
		RenderCommand::SetViewport(0, 0, GetWidth(), GetHeight());
	}

	void GLFWWindow::SetVSync(bool value)
	{
	}

	int GLFWWindow::GetVSync()
	{
		return 0;
	}

	void GLFWWindow::HandleEvents()
	{
	}
}