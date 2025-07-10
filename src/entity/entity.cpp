#include "entity/entity.hpp"

#include "raylib.h"

float Entity::GetX() {
  return m_Position.x;
}

float Entity::GetY() {
  return m_Position.y;
}

Vector2& Entity::GetPos() {
  return m_Position;
}
