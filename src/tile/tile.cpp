#include "tile.hpp"
#include <map>
#include <string>

#include "core/core.hpp"
#include "raylib.h"
#include "utils/types.hpp"
#ifdef BUILDER
#include "builder/builder.hpp"
#else
#include "game.hpp"
#endif

bool loadedTextures = false;

static std::map<std::string, Texture> textures = {};

Tile::Tile(std::string type, Vec2d position) {
    if (!loadedTextures) {
        textures["Stone"] = LoadTexture("Assets/stone.png");
        textures["Dirt"] = LoadTexture("Assets/dirt.png");
        textures["Grass"] = LoadTexture("Assets/grass.png");
        textures["Brick"] = LoadTexture("Assets/brick.png");
        textures["Grasspatch"] = LoadTexture("Assets/grass_patch.png");
        
        loadedTextures = true;
    }

    this->type = type;
    m_Position = position;

    this->args = TileArgs::NORMAL;

    size = 1.0f;

    texture = textures.at(type);

    m_ZOrder = 1.0f;
}

Tile::Tile(std::string type, Vec2d position, TileArgs args) {
    if (!loadedTextures) {
        textures["Stone"] = LoadTexture("Assets/stone.png");
        textures["Dirt"] = LoadTexture("Assets/dirt.png");
        textures["Grass"] = LoadTexture("Assets/grass.png");
        textures["Brick"] = LoadTexture("Assets/brick.png");
        textures["Grasspatch"] = LoadTexture("Assets/grass_patch.png");
        
        loadedTextures = true;
    }

    this->type = type;
    m_Position = position;

    this->args = args;

    size = 1.0f;

    texture = textures.at(type);

    m_ZOrder = 0.5f;
}

void Tile::OnRender() {
#ifdef BUILDER
    Builder::GetRenderer()->RenderTexture(texture, m_Position.x * 64, m_Position.y * 64);
#else
    Game::GetRenderer()->RenderTexture(texture, m_Position.x * 64, m_Position.y * 64);

    //Debug("Hello from instance!");
#endif
}

float Tile::GetX() const {
    return m_Position.x;
}

float Tile::GetY() const {
    return m_Position.y;
}

Texture& Tile::GetTexture() {
    return texture;
}

float Tile::GetSize() {
    return size;
}

Vec2d& Tile::GetPos() {
    return m_Position;
}

std::string Tile::GetTileType() {
    return type;
}

TileArgs Tile::GetTileArgs() {
    return args;
}