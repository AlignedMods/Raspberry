#pragma once

#include "core/instance.hpp"
#include "raylib.h"
#include "utils/types.hpp"

#include "pch.hpp"

enum class TileArgs { NORMAL, NO_CAMERA };

class Tile : public Instance {
 public:
  Tile() = default;
  Tile(std::string type, Vec2d tilePos);
  Tile(std::string type, Vec2d tilePos, TileArgs tileArgs);

  virtual void OnRender() override;
  float GetX() const;
  float GetY() const;

  Texture& GetTexture();

  Vec2d& GetPos();
  std::string GetTileType();

  Rectangle& GetRectangle();

  float GetSize();

  TileArgs GetTileArgs();

 private:
  std::string type;
  Vec2d tileSize;

  TileArgs args;

  Texture texture;

  float size;
};
