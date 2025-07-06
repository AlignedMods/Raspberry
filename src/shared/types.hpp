#pragma once

#include <cstdint>
#include "raylib.h"

struct TilePosition {
	uint16_t x;
	uint16_t y;

	inline Vector2 RaylibVector() {
		return Vector2{static_cast<float>(x), static_cast<float>(y)};
	}
};
