#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Engine {

	class Renderer2D
	{
	public:
		static void Init();

		static void StartFrame();
		static void EndFrame();

		/** Draw square
		@param position: Just your x and y position
		@param scale: x scale and y scale
		@param colour: r,g,b,a
		*/
		static void DrawQuad(glm::vec2 position, glm::vec2 scale, glm::vec4 colour);
		/** Draw square
		@param position: Just your x, y and z position
		@param scale: x scale and y scale
		@param colour: r,g,b,a
		*/
		static void DrawQuad(glm::vec3 position, glm::vec2 scale, glm::vec4 colour);

		/** Draw square with rotation
		@param position: Just your x and y position
		@param scale: x scale and y scale
		@param rotation: Rotation in degrees
		@param colour: r,g,b,a
		*/
		static void DrawQuad(glm::vec2 position, glm::vec2 scale, float rotation, glm::vec4 colour);
		/** Draw square with rotation
		@param position: Just your x, y and z position
		@param scale: x scale and y scale
		@param rotation: Rotation in degrees
		@param colour: r,g,b,a
		*/
		static void DrawQuad(glm::vec3 position, glm::vec2 scale, float rotation, glm::vec4 colour);
		static void DrawQuad(glm::vec3 position, glm::vec2 scale, std::string filepath, glm::vec3 tintColour);
	};
}

