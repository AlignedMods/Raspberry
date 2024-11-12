#include "player.hpp"
#include "game.hpp"
#include "raylib.h"

Player::Player()
{
    m_Pos.x = 0.0f;
    m_Pos.y = 0.0f;

    m_Size = 64.0f;
    m_Speed = 0.5f;

    m_Jumping = false;

    m_Velocity = { 0.0f, 0.0f };

    m_Level = Game::Get()->GetCurrentLevel();
}

void Player::Update()
{
    if (!m_OnGround) {
        m_Velocity.y += m_Gravity;
    }

    m_Pos.x += m_Velocity.x;
    m_Pos.y += m_Velocity.y;

    if (IsKeyDown(KEY_D)) {
        m_Velocity.x += m_Speed;
    } else if (IsKeyDown(KEY_A)) {
        m_Velocity.x -= m_Speed;
    } else {
        if (m_Velocity.x < 0) {
            m_Velocity.x += m_Speed;
        } else if (m_Velocity.x > 0) {
            m_Velocity.x -= m_Speed;
        }
    }

    if (IsKeyPressed(KEY_SPACE)) {
        Jump();
    }

    CheckCollisions();
}

void Player::CheckCollisions()
{
    auto tiles = m_Level->GetAllTiles();

    // Flags to track different collision states
    bool isGroundCollision = false;
    bool isLeftCollision = false;
    bool isRightCollision = false;

    Vector2 finalPos = { m_Pos.x, m_Pos.y };

    for (auto rec : tiles) {
        if (CheckCollisionRecs({ m_Pos.x, m_Pos.y, m_Size, m_Size },
                { rec.GetX(), rec.GetY(), rec.GetSize(), rec.GetSize() })) {
            // Collision from the top
            if (m_Velocity.y > 0) {
                finalPos.y = rec.GetY() - m_Size;
                m_Velocity.y = 0;
                isGroundCollision = true;
                m_Jumping = false;
            }
        }
    }

    m_Pos.y = finalPos.y;
    m_OnGround = isGroundCollision;

    // Check horizontal collisions separately
    for (auto rec : tiles) {
        // Only check for horizontal collisions if the player is on the ground
        // (or if that's your intended behavior)
        if (CheckCollisionRecs({ m_Pos.x, m_Pos.y, m_Size, m_Size },
                { rec.GetX(), rec.GetY(), rec.GetSize(), rec.GetSize() })) {
            // Going right
            if (m_Velocity.x > 0 && m_Pos.x + m_Size >= rec.GetX()) {
                m_Velocity.x = 0;
                finalPos.x = rec.GetX() - m_Size;
                isRightCollision = true;
            }
            // Going left
            else if (m_Velocity.x < 0
                && m_Pos.x <= rec.GetX() + rec.GetSize()) {
                m_Velocity.x = 0;
                finalPos.x = rec.GetX() + rec.GetSize();
                isLeftCollision = true;
            }
        }
    }

    m_Pos.x = finalPos.x;
}

float Player::GetX()
{
    return m_Pos.x;
}

float Player::GetY()
{
    return m_Pos.y;
}

void Player::Draw()
{
    DrawRectangle(m_Pos.x, m_Pos.y, m_Size, m_Size, RED);
}

void Player::Jump()
{
    if (m_OnGround || IsTileBelowPlayer()) {
        m_Velocity.y -= 10.0f;
        m_Jumping = true;
        m_OnGround = false;
    }
}

bool Player::IsTileBelowPlayer()
{
    auto tiles = m_Level->GetAllTiles();

    for (auto tile : tiles) {
        if (tile.GetY() == m_Pos.y + m_Size - 0.5f) {
            return true;
        }
    }

    return false;
}
