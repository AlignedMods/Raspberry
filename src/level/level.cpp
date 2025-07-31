#include "level.hpp"
#include "collectables/collectable.hpp"
#include "collectables/raspberry.hpp"
#include "game.hpp"
#include "raylib.h"
#include "tile/tile.hpp"
#include "core/log.hpp"

#include <cstdint>
#include <format>
#include <fstream>
#include <string>
#include <sstream>

void Level::OnUpdate() {
    m_Player.OnUpdate();
    m_Collectable.OnUpdate();

    for (auto& mob : m_Mobs) {
        mob.OnUpdate();
    }
}

void Level::OnRender() {
    int location = GetShaderLocation(m_Shader, "brightness");
    SetShaderValue(m_Shader, location, (float*)&m_Brightness, SHADER_UNIFORM_FLOAT);

    float v = 0.0f;

    i32 counter = 0;
    const Rectangle& vp = Game.GetViewportRect();

    BeginShaderMode(m_Shader);

    for (auto& tile : m_Tiles) {
        if (CheckCollisionRecs({tile.GetRealPosition().x, tile.GetRealPosition().y, 64.0f, 64.0f}, vp)) {
            counter++;
            tile.OnRender();
        }
    }

    m_Player.OnRender();
    m_Collectable.OnRender();

    for (auto& mob : m_Mobs) {
        mob.OnRender();
    }

    EndShaderMode();

    Log(LogLevel::Info, std::format("Rendering {} tiles", counter));
}

void Level::Tick() {
    m_Player.UpdateTextures();
}

Player& Level::GetPlayer() {
    return m_Player;
}

void Level::AddTile(Tile tile) {
    m_Tiles.push_back(tile);
}

std::vector<Tile>& Level::GetAllTiles() {
    return m_Tiles;
}

void Level::AddCollectable() {
    m_Collectable = Raspberry();
}

Collectable& Level::GetCollectable() {
    return m_Collectable;
}

void Level::SetFound() {
    m_CollectableFound = true;
}

bool Level::IsCollectableFound() {
    return m_CollectableFound;
}

bool Level::LoadLevelFromFile(std::filesystem::path path) {
    std::ifstream file(path, std::ios::binary);

    if (!file.good()) {
        return false;
    }

    std::stringstream contentsStream;
    std::string contents;

    contentsStream << file.rdbuf();
    contents = contentsStream.str();

    file.close();

    int pos = 0;

    // a string used to check any string in the file
    std::string str;

    // check if the file is valid (contains RSP at the start)
    while (pos < 3) {
        str.append(1, contents.at(pos++));
    }

    Log(LogLevel::Info, str);

    if (str != "RSP") {
        Log(LogLevel::Warning, "Not a valid .rsp file!");
        return false;
    }

    str.clear();

    // Get the size of the name
    uint8_t nameSize = contents.at(pos++);

    // Get the actual name
    for (int i = 0; i < nameSize; i++) {
        m_Name.append(1, contents.at(pos));
        pos++;
    }

    size_t end = pos + 5;

    // Get the _H_E_ string
    while (pos < end) {
        str.append(1, contents.at(pos++));
    }

    if (str != "_H_E_") {
        Log(LogLevel::Warning, ".rsp File Header doesn't contain \"_H_E_\" at the correct spot!");
        return false;
    }

    str.clear();

    while (pos < contents.size()) {
        while (contents.at(pos) != ':') {
            str.append(1, contents.at(pos));
            pos++;
        }

        Log(LogLevel::Info, str);

        // we are parsing a tile
        if (str == "tile") {
            // skip past the ':'
            pos++;

            std::string type;
            int32_t x;
            int32_t y;

            while (contents.at(pos) != '\\') {
                type.append(1, contents.at(pos));
                pos++;
            }

            pos++;

            // reassemble the number from little endian
            x = static_cast<int32_t>((int8_t)contents.at(pos + 3) << 24 |
                                     (int8_t)contents.at(pos + 2) << 16 |
                                     (int8_t)contents.at(pos + 1) << 8  |
                                     (int8_t)contents.at(pos));

            pos += 4;

            y = static_cast<int32_t>((int8_t)contents.at(pos + 3) << 24 |
                                     (int8_t)contents.at(pos + 2) << 16 |
                                     (int8_t)contents.at(pos + 1) << 8  |
                                     (int8_t)contents.at(pos));

            pos += 4;

            Log(LogLevel::Info, std::format("{}, {}", x, y));

            AddTile(Tile(type, {x, y}));
        }

        str.clear();
    }

    m_Player.InitTextures();

    m_Shader = LoadShader(nullptr, "Assets/Shaders/lighting.glsl");

    //m_Mobs.push_back(Mob());
    //m_Mobs.begin()->InitTextures();

    Log(LogLevel::Info, "Successfully loaded level!");

    return true;
}
