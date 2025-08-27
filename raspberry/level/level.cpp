#include "level.hpp"
#include "game.hpp"
#include "log.hpp"
#include "raylib.h"
#include "tile/tile.hpp"

#include <format>
#include <string>

void Level::OnUpdate() {
    m_Player.OnUpdate();

    for (auto &mob : m_Mobs) {
        mob.OnUpdate();
    }
}

void Level::OnRender() {
    // int location = GetShaderLocation(m_Shader, "brightness");
    // SetShaderValue(m_Shader, location, (float*)&m_Brightness, SHADER_UNIFORM_FLOAT);

    float v = 0.0f;

    i32 counter = 0;
    const Rectangle &vp = Game.GetViewportRect();

    // BeginShaderMode(m_Shader);

    for (auto &tile : m_Tiles) {
        if (CheckCollisionRecs({tile.GetRealPosition().x, tile.GetRealPosition().y, 32.0f, 32.0f}, vp)) {
            counter++;
            tile.OnRender();
        }
    }

    m_Player.OnRender();

    for (auto &mob : m_Mobs) {
        mob.OnRender();
    }

    // EndShaderMode();

    Log(Log_Info, "Rendering %d tiles", counter);
}

void Level::Tick() {
    m_Player.UpdateTextures();
}

Player &Level::GetPlayer() {
    return m_Player;
}

std::vector<Mob> &Level::GetAllMobs() {
    return m_Mobs;
}

void Level::AddTile(Tile tile) {
    m_Tiles.push_back(tile);
}

std::vector<Tile> &Level::GetAllTiles() {
    return m_Tiles;
}

void Level::SetFound() {
    m_CollectableFound = true;
}

bool Level::IsCollectableFound() {
    return m_CollectableFound;
}

void Level::SetName(const std::string &name) {
    m_Name = name;
}

std::string &Level::GetName() {
    return m_Name;
}