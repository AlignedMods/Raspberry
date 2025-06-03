#include "entity/entity.hpp"
#include "game.hpp"
#include "raylib.h"
#include "utils/types.hpp"

float Entity::GetX() {
  return m_Position.x;
}

float Entity::GetY() {
  return m_Position.y;
}

Vec2d& Entity::GetPos() {
  return m_Position;
}

void Entity::OnRender() {
    Game::Get()->GetRenderer()->RenderTexture(LoadTexture("Assets/grass.png"), m_Position.x, m_Position.y);
}