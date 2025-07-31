#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Engine {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_arrayID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_arrayID);
	}

	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_arrayID);
	}

	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
}