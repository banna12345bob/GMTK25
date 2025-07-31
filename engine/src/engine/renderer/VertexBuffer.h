#pragma once

#include "engine/core/core.h"

#include <glm/glm.hpp>

namespace Engine {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(glm::vec4* vertices, uint32_t size);
	};
}

