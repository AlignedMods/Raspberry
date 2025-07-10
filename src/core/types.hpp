#pragma once

#include "raylib.h"

#include <cstdint>

struct TilePosition {
	int16_t x;
	int16_t y;

	inline Vector2 RaylibVector() {
		return Vector2{static_cast<float>(x), static_cast<float>(y)};
	}
};
