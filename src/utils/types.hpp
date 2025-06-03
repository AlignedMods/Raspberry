#pragma once

#include "raylib.h"

class Vec2d {
public:
    Vec2d();
    Vec2d(float x, float y);
    Vector2 ToRaylibVector();
    void Zero();

    float x;
    float y;
};

class Rec2d {
public:
    Rec2d();
    Rec2d(float x, float y, float width, float height);
    Rec2d(const Vec2d& position, const Vec2d& size);
    Rectangle ToRaylibrectangle();
    void Zero();

    Vec2d position;
    Vec2d size;
};