#pragma once

#include <pch.hpp>

#include "entity/entity.hpp"
#include "level/level.hpp"
#include "tile/tile.hpp"

class Level;

class Player : public Entity {
 public:
  Player();

  void operator=(const Player& player);

  virtual void OnRender() override;
  virtual void OnUpdate() override;

  void Jump();
  std::optional<Tile> GetTileBelowPlayer();

 private:
  void CheckCollisions();

 private:
  float m_Speed;

  const float m_Gravity = 0.02f;

  bool m_Jumping;
  bool m_OnGround;

  std::shared_ptr<Level> m_Level = nullptr;
};
