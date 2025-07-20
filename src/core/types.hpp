#pragma once

#include "core/log.hpp"
#include "core/util.hpp"

#include "raylib.h"

#include <cstddef>
#include <cstdint>
#include <format>

struct TilePosition {
    int32_t x;
    int32_t y;

    inline Vector2 RaylibVector() {
        return Vector2{static_cast<float>(x), static_cast<float>(y)};
    }
};

inline bool operator<(const TilePosition& f, const TilePosition s) {
    if (f.x != s.x) {
        return f.x < s.x;
    }

    return f.y < s.y;
}
