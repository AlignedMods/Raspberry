#pragma once

#include "raylib.h"

class Entity {
 public:
  virtual ~Entity() {}
  virtual void OnRender() {}
  virtual void OnUpdate() {}

  float GetX();
  float GetY();

  Vector2& GetPos();

 protected:
  Vector2 m_Pos;
  Vector2 m_Velocity;
  Rectangle m_Rectangle;

  Texture m_Texture;

  void Move(const Vector2& pos);
};
