// chat GPT our lord and saviour

#include "player.hpp"

#include <optional>

#include "Vector2.hpp"
#include "core/core.hpp"
#include "game.hpp"
#include "raylib.h"
#include "raymath.h"
#include "tile/tile.hpp"

Player::Player() {
  m_Pos.x = 0.0f;
  m_Pos.y = 0.0f;

  m_Speed = 0.02;

  m_Jumping = false;

  m_Velocity = raylib::Vector2(0.0f, 0.0f);

  m_Level = Game::Get()->GetCurrentLevel();

  m_Rectangle = {m_Pos.x, m_Pos.y, 1.0f, 1.0f};
}

void Player::OnUpdate() {
  if (!m_OnGround) {
    m_Velocity.y += m_Gravity;
  }

  m_Pos.x += m_Velocity.x;
  m_Pos.y += m_Velocity.y;

  m_Rectangle = {m_Pos.x, m_Pos.y, 1.0f, 1.0f};

  if (IsKeyDown(KEY_D)) {
    m_Velocity.x += m_Speed;
  } else if (IsKeyDown(KEY_A)) {
    m_Velocity.x -= m_Speed;
  } else if (IsKeyDown(KEY_S)) {
    m_Velocity.y += m_Speed;
  } else if (IsKeyDown(KEY_W)) {
    m_Velocity.y -= m_Speed;
  } else {
    if (m_Velocity.x < 0.0f) {
      m_Velocity.x += m_Speed;
    } else if (m_Velocity.x > 0.0f) {
      m_Velocity.x -= m_Speed;
    }
  }

  if (IsKeyDown(KEY_SPACE)) {
    Debug("Jump!");
    Jump();
  }

  CheckCollisions();
}

void Player::CheckCollisions() {
  auto& tiles = m_Level->GetAllTiles();

  // Flags to track different collision states
  bool isGroundCollision = false;

  Vector2 finalPos = {m_Pos.x, m_Pos.y};

  for (auto& tile : tiles) {
    if (CheckCollisionRecs(m_Rectangle, tile.GetRectangle())) {
      Rectangle& rec = tile.GetRectangle();
      // Calculate overlap on each axis
      float overlapX =
          std::min(m_Rectangle.x + m_Rectangle.width, rec.x + rec.width) -
          std::max(m_Rectangle.x, rec.x);
      float overlapY =
          std::min(m_Rectangle.y + m_Rectangle.height, rec.y + rec.height) -
          std::max(m_Rectangle.y, rec.y);

      // Resolve the collision based on the smallest overlap
      if (overlapX < overlapY) {
        // Horizontal collision
        if (m_Rectangle.x < rec.x) {
          Debug("Collision from right!");
          m_Velocity.x = 0.0f;
          finalPos.x = rec.x - rec.width;
        } else {
          Debug("Collision from left!");
          m_Velocity.x = 0.0f;
          finalPos.x = rec.x + rec.width;
        }
      } else {
        // Vertical collision
        if (m_Rectangle.y < rec.y) {
          Debug("Collision from up!");
          m_Velocity.y = 0.0f;
          finalPos.y = rec.y - rec.height;
          isGroundCollision = true;
          m_Jumping = false;
        } else {
          Debug("Collision from down!");
          m_Velocity.y = 0.0f;
          finalPos.y = rec.y + rec.height;
        }
      }
    }

    m_Pos.y = finalPos.y;
    m_OnGround = isGroundCollision;

    m_Pos.x = finalPos.x;
  }
}

void Player::OnRender() {
  DrawRectangle(m_Pos.x * 64, m_Pos.y * 64, 1.0f * 64, 1.0f * 64, RED);

  std::stringstream tempStr;

  tempStr << "Drawing player at: " << m_Pos.x << ", " << m_Pos.y;

  // Debug(tempStr.str());
}

void Player::Jump() {
  if (m_OnGround && !m_Jumping) {
    // This value is exponential
    m_Velocity.y -= 0.3f;
    m_Jumping = true;
    m_OnGround = false;
  }
}

std::optional<Tile> Player::GetTileBelowPlayer() {
  auto& tiles = m_Level->GetAllTiles();

  for (auto& tile : tiles) {
    if (Vector2Equals(
            {tile.GetPos().x, tile.GetPos().y},
            Vector2Add({m_Pos.x, std::ceil(m_Pos.y)}, {0.0f, 1.0f}))) {
      return tile;
    }
  }

  return std::nullopt;
}
