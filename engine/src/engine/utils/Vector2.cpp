#include "Vector2.h"
#include <format>

namespace Engine {

	Vector2::Vector2() {
		x = 0;
		y = 0;
	}
	Vector2::Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	std::string Vector2::ToString() {
		return std::format("X: {}, Y: {}", x, y);
	}

	Vector2i::Vector2i() {
		x = 0;
		y = 0;
	}
	Vector2i::Vector2i(int x, int y) {
		this->x = x;
		this->y = y;
	}
	std::string Vector2i::ToString() {
		return std::format("X: {}, Y: {}", x, y);
	}
}