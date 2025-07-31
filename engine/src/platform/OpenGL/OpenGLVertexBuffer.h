#pragma once

#include "engine/renderer/VertexBuffer.h"

namespace Engine {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(glm::vec4* vertices, uint32_t size);

		virtual ~OpenGLVertexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		unsigned int m_BufferID;
	};
}

