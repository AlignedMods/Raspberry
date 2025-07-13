#pragma once

#include "raylib.h"

#include <cstdint>

struct TilePosition {
	int32_t x;
	int32_t y;

	inline Vector2 RaylibVector() {
		return Vector2{static_cast<float>(x), static_cast<float>(y)};
	}
};
