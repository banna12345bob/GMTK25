#pragma once

#include "engine/core/core.h"

namespace Engine {

	class VertexArray
	{
	public:
		~VertexArray() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<VertexArray> Create();
	};

}

