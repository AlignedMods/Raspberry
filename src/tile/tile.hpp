#pragma once

#include "Rectangle.hpp"
#include "Texture.hpp"
#include "Vector2.hpp"
#include "pch.hpp"
#include "raylib.h"

enum class TileType { AIR, DIRT, STONE, GRASS, BRICK };

enum class TileArgs { NORMAL, NO_CAMERA };

class Tile {
 public:
  Tile() = default;
  Tile(TileType tileType, Vector2 tilePos);
  Tile(TileType tileType, Vector2 tilePos, TileArgs tileArgs);

  void Draw();
  float GetX() const;
  float GetY() const;

  Texture& GetTexture();

  Vector2& GetPos();
  TileType GetTileType();

  Rectangle& GetRectangle();

  float GetSize();

  TileArgs GetTileArgs();

 private:
  TileType m_TileType;
  Vector2 m_Pos;
  Vector2 m_TileSize;

  Rectangle m_Rectangle;

  TileArgs m_TileArgs;

  Texture m_Texture;

  float m_Size;
};
