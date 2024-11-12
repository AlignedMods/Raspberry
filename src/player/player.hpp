#pragma once

#include "core/core.hpp"
#include "level/level.hpp"
#include "raylib.h"

#include <pch.hpp>

enum class Direction {
    nowhere = 0,
    Up,
    Down,
    Right,
    Left,
    DownRight,
    DownLeft
};

class Level;

class Player {
public:
    Player();

    void operator=(const Player& player);

    void Update();
    void Draw();
    float GetX();
    float GetY();

    void Jump();
    bool IsTileBelowPlayer();

private:
    void CheckCollisions();

    Vector2 m_Pos;

    float m_Size;
    float m_Speed;

    CollisionBox m_CollisonBox;

    const float m_Gravity = 0.5f;

    Vector2 m_Velocity;

    bool m_Jumping;
    bool m_OnGround;

    Texture2D m_Texture;
    std::shared_ptr<Level> m_Level = nullptr;
};
