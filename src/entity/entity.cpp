#include "entity/entity.hpp"

void Entity::Move(const Vector2& pos) {
  if (m_Pos.x > pos.x) {
    m_Velocity.x -= 0.1;
  } else if (m_Pos.x < pos.x) {
    m_Velocity.x += 0.1;
  }

  if (m_Pos.y > pos.y) {
    m_Velocity.y -= 0.1;
  } else if (m_Pos.y < pos.y) {
    m_Velocity.y += 0.1;
  }
}

float Entity::GetX() {
  return m_Pos.x;
}

float Entity::GetY() {
  return m_Pos.y;
}

Vector2& Entity::GetPos() {
  return m_Pos;
}
