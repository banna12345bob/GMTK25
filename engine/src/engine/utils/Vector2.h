#pragma once
#include <string>

namespace Engine {

	struct Vector2
	{
	public:
		Vector2();
		Vector2(float x, float y);

		std::string ToString();

		float x;
		float y;
	};

	struct Vector2i
	{
	public:
		Vector2i();
		Vector2i(int x, int y);

		std::string ToString();

		int x;
		int y;
	};
}