#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>


#include "engine/renderer/VertexArray.h"
#include "engine/renderer/Buffers.h"
#include "engine/renderer/Shader.h"

namespace Engine {

	struct SquareVertex
	{
		glm::vec3 position;
		glm::vec4 colour;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;
	};

	struct Renderer2DData
	{
		static const uint32_t maxQuads = 20000;
		static const uint32_t maxVerticies = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> ColourShader;

		uint32_t squareIndexCount = 0;

		SquareVertex* squareVertexBufferBase = nullptr;
		SquareVertex* squareVertexBufferPtr = nullptr;

		uint32_t textureSlotIndex = 1;

		glm::vec4 quadVertexPosition[4];
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.maxVerticies * sizeof(SquareVertex));
		s_Data.QuadVertexBuffer->SetLayout(BufferLayout({
			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float4, "aColour" },
			{ ShaderDataType::Float2, "aTexCoord" },
			{ ShaderDataType::Float, "aTexIndex" },
			{ ShaderDataType::Float, "aTilingFactor" }
			}));
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.squareVertexBufferBase = new SquareVertex[s_Data.maxVerticies];

		uint32_t* squareIndices = new uint32_t[s_Data.maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.maxIndices; i += 6)
		{
			squareIndices[i + 0] = offset + 0;
			squareIndices[i + 1] = offset + 1;
			squareIndices[i + 2] = offset + 2;

			squareIndices[i + 3] = offset + 2;
			squareIndices[i + 4] = offset + 3;
			squareIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(squareIndices, s_Data.maxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		delete[] squareIndices;

		s_Data.ColourShader = Shader::Create("assets/shaders/colour.glsl");

		s_Data.quadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPosition[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::StartFrame() 
	{
		s_Data.ColourShader->Bind();

		s_Data.squareIndexCount = 0;
		s_Data.squareVertexBufferPtr = s_Data.squareVertexBufferBase;
	}

	void Renderer2D::EndFrame()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.squareVertexBufferPtr - (uint8_t*)s_Data.squareVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.squareVertexBufferBase, dataSize);

		s_Data.ColourShader->Bind();
		uint32_t count = s_Data.squareIndexCount ? s_Data.squareIndexCount : s_Data.QuadVertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 scale, glm::vec4 colour)
	{
		DrawQuad({ position.x, position.y, 0 }, scale, colour);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 scale, glm::vec4 colour)
	{
		DrawQuad(position, scale, 0, colour);
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 scale, float rotation, glm::vec4 colour)
	{
		DrawQuad({ position.x, position.y, 0 }, scale, rotation, colour);
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 scale, float rotation, glm::vec4 colour)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		constexpr size_t squareVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const float texIndex = 0.0f;

		const float tilingFactor = 1.0f;

		for (size_t i = 0; i < squareVertexCount; i++)
		{
			s_Data.squareVertexBufferPtr->position = transform * s_Data.quadVertexPosition[i];
			s_Data.squareVertexBufferPtr->colour = colour;
			s_Data.squareVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.squareVertexBufferPtr->texIndex = texIndex;
			s_Data.squareVertexBufferPtr->tilingFactor = tilingFactor;
			s_Data.squareVertexBufferPtr++;
		}

		s_Data.squareIndexCount += 6;
	}

	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 scale, std::string filepath, glm::vec3 tintColour)
	{
		EG_CORE_WARN("Needs to be implimented");
	}
}