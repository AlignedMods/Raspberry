#include "tile.hpp"
#include "core/core.hpp"
#include "raylib.h"

#ifdef BUILDER
#include "builder/builder.hpp"
#else
#include "game.hpp"
#endif

Tile::Tile(TileType tileType, Vector2 tilePos)
{
    m_TileType = tileType;
    m_TilePos = tilePos;

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

    m_Size = 64;

    m_X = m_TilePos.x * m_Size;
    m_Y = m_TilePos.y * m_Size;
}

Tile::Tile(TileType tileType, Vector2 tilePos, TileArgs tileArgs)
{
    m_TileType = tileType;
    m_TilePos = tilePos;

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

    m_Size = 64;

    m_X = m_TilePos.x * m_Size;
    m_Y = m_TilePos.y * m_Size;
}

void Tile::Draw()
{
#ifdef BUILDER
    Builder::GetRenderer()->RenderTexture(m_Texture, m_X, m_Y);
#else
    Game::GetRenderer()->RenderTexture(m_Texture, m_X, m_Y);
#endif
}

float Tile::GetX()
{
    return m_X;
}

float Tile::GetY()
{
    return m_Y;
}

Texture2D Tile::GetTexture()
{
    return m_Texture;
}

float Tile::GetSize()
{
    return m_Size;
}

Vector2 Tile::GetPos()
{
    return m_TilePos;
}

CollisionBox& Tile::GetCollisonBox()
{
    return m_CollisonBox;
}

TileType Tile::GetTileType()
{
    return m_TileType;
}

TileArgs Tile::GetTileArgs()
{
    return m_TileArgs;
}
