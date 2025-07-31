#include "VertexBuffer.h"

#include "engine/renderer/RenderAPI.h"

#include "platform/OpenGL/OpenGLVertexBuffer.h"

namespace Engine {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RenderAPI::getAPI()) {
		case RenderAPI::API::None: EG_CORE_ASSERT(false, "Cannot have RenderAPI::None");
		case RenderAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
		}

		EG_CORE_ASSERT(false, "Unknown renderAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RenderAPI::getAPI()) {
		case RenderAPI::API::None: EG_CORE_ASSERT(false, "Cannot have RenderAPI::None");
		case RenderAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		EG_CORE_ASSERT(false, "Unknown renderAPI");
		return nullptr;
	}
}