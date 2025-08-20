#include "registry/registry.hpp"
#include "raylib.h"
#include "types.hpp"
#include "log.hpp"

#include <fstream>

void s_Registry::RegisterArchive(const std::filesystem::path& archive) {
    Log(LogLevel::Info, std::format("Adding archive {}", archive.string()));

    std::ifstream file(archive, std::ios::binary);

    std::stringstream contentsStream;
    std::string contents;

    std::string type;
    std::string name;
    std::string data;
    u32 size;

    std::string temp;

    contentsStream << file.rdbuf();

    contents = contentsStream.str();
    contentsStream.flush(); // bye buffer
    
    u64 index = 0;

    while (index < contents.size()) {
        while (contents.at(index) != '\n') {
            temp += contents.at(index);
            index++;
        }

        index++;
        
        type = temp;
        temp.clear();

        while (contents.at(index) != '\n') {
            temp += contents.at(index);
            index++;
        }

        index++;

        name = temp;
        temp.clear();

        while (contents.at(index) != '\n') {
            temp += contents.at(index);
            index++;
        }

        index++;

        size = std::stoi(temp);
        temp.clear();

        u64 counter = 0;

        while (counter < size) {
            temp += contents.at(index + counter);
            counter++;
        }

        index += counter;

        data = temp;
        temp.clear();

        if (type == "texture") {
            RegisterTexture(name, data);
        }

        if (type == "font") {
        }

        if (type == "map") {
            RegisterLevel(name, data);
        }

        if (type == "sound") {
            RegisterSound(name, data);
        }

        index++;
    }
}


void s_Registry::RegisterTexture(const std::string& name, const std::string& pngData) {
    Image temp = LoadImageFromMemory(".png", (u8*)(name.c_str()), name.size());

    m_Textures[name.c_str()] = LoadTextureFromImage(temp);
    UnloadImage(temp);
}

void s_Registry::RegisterLevel(const std::string& name, const std::string& lvlData) {
    std::string contents = std::move(lvlData);
    Level l;

    int pos = 0;

    // a string used to check any string in the file
    std::string str;

    // check if the file is valid (contains RSP at the start)
    while (pos < 3) {
        str += contents.at(pos++);
    }

    Log(LogLevel::Info, str);

    if (str != "RSP") {
        Log(LogLevel::Warning, "Not a valid .lvl file!");
        return;
    }

    str.clear();

    // Get the size of the name
    uint8_t nameSize = contents.at(pos++);

    // Get the actual name
    for (int i = 0; i < nameSize; i++) {
        str += contents.at(pos);
        pos++;
    }

    l.SetName(str);
    str.clear();

    size_t end = pos + 5;

    // Get the _H_E_ string
    while (pos < end) {
        str += contents.at(pos++);
    }

    if (str != "_H_E_") {
        Log(LogLevel::Warning, ".rsp File Header doesn't contain \"_H_E_\" at the correct spot!");
        return;
    }

    str.clear();

    while (pos < contents.size()) {
        while (contents.at(pos) != ':') {
            str += contents.at(pos);
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

            l.AddTile(Tile(type, {x, y}));
        }

        str.clear();
    }

    // m_Shader = LoadShader(nullptr, "Assets/Shaders/lighting.glsl");

    //m_Mobs.push_back(Mob());
    //m_Mobs.begin()->InitTextures();

    l.GetPlayer().InitTextures();
    
    for (auto& mob : l.GetAllMobs()) {
        mob.InitTextures();
    }

    Log(LogLevel::Info, "Successfully loaded level!");

    m_Levels[name.c_str()] = l;
}

void s_Registry::RegisterSound(const std::string& name, const std::string& wavData) {
    Wave temp = LoadWaveFromMemory(".wav", (u8*)(wavData.c_str()), wavData.size());

    m_Sounds[name.c_str()] = LoadSoundFromWave(temp);
    UnloadWave(temp);
}

Texture& s_Registry::GetTexture(const std::string& name) {
    return m_Textures.at(name.c_str());
}

Level& s_Registry::GetLevel(const std::string& name) {
    return m_Levels.at(name.c_str());
}

Sound& s_Registry::GetSound(const std::string& name) {
    return m_Sounds.at(name.c_str());
}

void s_Registry::PlaySound(const std::string& name) {
    ::PlaySound(GetSound(name));
}
