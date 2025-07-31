#pragma once

#include "engine/core/core.h"

namespace Engine {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};
}

