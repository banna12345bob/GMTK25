#pragma once

#include "engine/renderer/VertexBuffer.h"

namespace Engine {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);

		virtual ~OpenGLVertexBuffer();

	private:
		unsigned int m_BufferID;
	};
}

