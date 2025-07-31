#include "Texture.h"

#include "RenderAPI.h"

#include "platform/OpenGL/OpenGLTexture.h"

namespace Engine {

	Ref<Texture2D> Engine::Texture2D::Create(const uint32_t width, const uint32_t height)
	{
		switch (RenderAPI::getAPI()) {
		case RenderAPI::API::None: EG_CORE_ASSERT(false, "Cannot have RenderAPI::None");
		case RenderAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
		}

		EG_CORE_ASSERT(false, "Unknown renderAPI");
		return nullptr;
	}

	Ref<Texture2D> Engine::Texture2D::Create(const std::string& path)
	{
		switch (RenderAPI::getAPI()) {
		case RenderAPI::API::None: EG_CORE_ASSERT(false, "Cannot have RenderAPI::None");
		case RenderAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}

		EG_CORE_ASSERT(false, "Unknown renderAPI");
		return nullptr;
	}
}