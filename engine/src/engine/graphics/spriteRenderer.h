#pragma once

#include "engine/core/core.h"

namespace Engine {
	class SpriteRenderer
	{
	public:
		static Scope<SpriteRenderer> Create();

		virtual void Render() {}
	};
}