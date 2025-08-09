#pragma once

#include "raylib.h"

#include <cstddef>
#include <cstdint>
#include <string>

// rust style types (no i still don't like rust, although type names is something they got right)

// integer types
using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

// floating point types
using f32 = float;
using f64 = double;

// size type
using sz = size_t;

using Hex = u32;

struct IVector2 {
    i32 x;
    i32 y;

    inline Vector2 RaylibVector() {
        return Vector2{static_cast<float>(x), static_cast<float>(y)};
    }
};

struct GuiInfo {
    Rectangle Bounds;
    std::string AnchorX;
    std::string AnchorY;

    bool Active;
};

// vectors
using f2 = Vector2;
using i2 = IVector2;

// operators
inline bool operator<(const i2& f, const i2& s) {
    if (f.x != s.x) {
        return f.x < s.x;
    }

    return f.y < s.y;
}

inline bool operator<(const f2& f, const f2& s) {
    if (f.x != s.x) {
        return f.x < s.x;
    }

    return f.y < s.y;
}
