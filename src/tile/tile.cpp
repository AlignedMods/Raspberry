#include "tile.hpp"

#include "raylib.h"
#ifdef BUILDER
#include "builder/builder.hpp"
#else
#include "game.hpp"
#endif

Tile::Tile(TileType tileType, Vector2 tilePos) {
  m_TileType = tileType;
  m_Pos = tilePos;

  m_TileArgs = TileArgs::NORMAL;

  if (m_TileType == TileType::STONE) {
    m_Texture = LoadTexture("Assets/stone.png");
  } else if (m_TileType == TileType::DIRT) {
    m_Texture = LoadTexture("Assets/dirt.png");
  } else if (m_TileType == TileType::GRASS) {
    m_Texture = LoadTexture("Assets/grass.png");
  } else if (m_TileType == TileType::BRICK) {
    m_Texture = LoadTexture("Assets/brick.png");
  }

  m_Size = 1.0f;

  m_Rectangle = {m_Pos.x, m_Pos.y, m_Size, m_Size};
}

Tile::Tile(TileType tileType, Vector2 tilePos, TileArgs tileArgs) {
  m_TileType = tileType;
  m_Pos = tilePos;

  m_TileArgs = tileArgs;

  if (m_TileType == TileType::STONE) {
    m_Texture = LoadTexture("Assets/stone.png");
  } else if (m_TileType == TileType::DIRT) {
    m_Texture = LoadTexture("Assets/dirt.png");
  } else if (m_TileType == TileType::GRASS) {
    m_Texture = LoadTexture("Assets/grass.png");
  } else if (m_TileType == TileType::BRICK) {
    m_Texture = LoadTexture("Assets/brick.png");
  }

  m_Size = 1.0f;

  m_Rectangle = {m_Pos.x, m_Pos.y, m_Size, m_Size};
}

void Tile::Draw() {
#ifdef BUILDER
  Builder::GetRenderer()->RenderTexture(m_Texture, m_Pos.x * 64, m_Pos.y * 64);
#else
  Game::GetRenderer()->RenderTexture(m_Texture, m_Pos.x * 64, m_Pos.y * 64);
#endif
}

float Tile::GetX() const {
  return m_Pos.x;
}

float Tile::GetY() const {
  return m_Pos.y;
}

Texture& Tile::GetTexture() {
  return m_Texture;
}

float Tile::GetSize() {
  return m_Size;
}

Vector2& Tile::GetPos() {
  return m_Pos;
}

Rectangle& Tile::GetRectangle() {
  return m_Rectangle;
}

TileType Tile::GetTileType() {
  return m_TileType;
}

TileArgs Tile::GetTileArgs() {
  return m_TileArgs;
}
