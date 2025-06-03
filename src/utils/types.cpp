#include "types.hpp"
#include "raylib.h"

// Vec2d

Vec2d::Vec2d() {
    Zero();
}

Vec2d::Vec2d(float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2 Vec2d::ToRaylibVector() {
    return Vector2(x, y);
}

void Vec2d::Zero() {
    x = 0.0f;
    y = 0.0f;
}

// Rec2d

Rec2d::Rec2d() {
    Zero();
}

Rec2d::Rec2d(float x, float y, float width, float height) {
    position.x = x;
    position.y = y;

    size.x = width;
    size.y = height;
}

Rec2d::Rec2d(const Vec2d& position, const Vec2d& size) {
    this->position = position;
    this ->size = size;
}


Rectangle Rec2d::ToRaylibrectangle() {
    return Rectangle(position.x, position.y, size.x, size.y);
}

void Rec2d::Zero() {
    position.Zero();
    size.Zero();
}