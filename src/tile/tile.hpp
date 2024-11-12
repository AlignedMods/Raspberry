#pragma once

#include "core/core.hpp"
#include "raylib.h"

enum class TileType { AIR, DIRT, STONE, GRASS, BRICK };

enum class TileArgs { NORMAL, NO_CAMERA };

class Tile {
public:
    Tile(TileType tileType, Vector2 tilePos);
    Tile(TileType tileType, Vector2 tilePos, TileArgs tileArgs);

    void Draw();
    float GetX();
    float GetY();

    Texture2D GetTexture();

    Vector2 GetPos();
    TileType GetTileType();

    CollisionBox& GetCollisonBox();

    float GetSize();

    TileArgs GetTileArgs();

private:
    TileType m_TileType;
    Vector2 m_TilePos;
    Vector2 m_TileSize;

    CollisionBox m_CollisonBox;

    TileArgs m_TileArgs;

    Texture2D m_Texture;

    float m_Size;

    float m_X;
    float m_Y;
};
