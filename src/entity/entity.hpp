#pragma once

#include "raylib.h"
#include "tile/tile.hpp"
#include "utils/types.hpp"

enum class EntityState {
    Running,
    Jumping,
    Freefalling,
    Idle
};

enum class Direction {
    Up,
    Down,
    Right,
    Left
};

class Entity : public Instance {
 public:
  virtual ~Entity() {}
  virtual void OnUpdate() {}

  virtual void OnRender() override;

  float GetX();
  float GetY();

  Vec2d& GetPos();

 protected:
  Vec2d m_Velocity;

  EntityState m_State;

  Texture m_Texture;

  int m_Anim = 0;
  Direction m_Direction = Direction::Down;
  bool m_TextureFlip = false;
};
