#include "Shader.h"

#include "RenderAPI.h"

#include "platform/OpenGL/OpenGLShader.h"

namespace Engine {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RenderAPI::getAPI()) {
		case RenderAPI::API::None: EG_CORE_ASSERT(false, "Cannot have RenderAPI::None");
		case RenderAPI::API::OpenGL: return CreateRef<OpenGLShader>(filepath);
		}

		EG_CORE_ASSERT(false, "Unknown renderAPI");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RenderAPI::getAPI()) {
		case RenderAPI::API::None: EG_CORE_ASSERT(false, "Cannot have RenderAPI::None");
		case RenderAPI::API::OpenGL: return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		EG_CORE_ASSERT(false, "Unknown renderAPI");
		return nullptr;
	}

}