#include "registry/registry.hpp"
#include "types.hpp"
#include "log.hpp"

#include "raylib.h"
#include "raymath.h"

#include <filesystem>
#include <fstream>

void s_Registry::RegisterArchive(const std::filesystem::path& archive) {
    Log(Log::Info, "Adding archive: %s", archive.c_str());

    if (!std::filesystem::exists(archive)) {
        Log(Log::Critical, "Archive does NOT exist!");
    }

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
            Log(Log::Info, "Adding texture with name: %s", name.c_str());
            RegisterTexture(name, data, size);
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


void s_Registry::RegisterTexture(const std::string& name, const std::string& pngData, const u64 size) {
    Image temp = LoadImageFromMemory(".png", (u8*)(pngData.c_str()), size);

    m_Textures[name] = LoadTextureFromImage(temp);
    UnloadImage(temp);
}

void s_Registry::RegisterLevel(const std::string& name, const std::string& lvlData) {
    std::string contents = lvlData;
    Level l;

    int pos = 0;

    // a string used to check any string in the file
    std::string str;

    // check if the file is valid (contains RSP at the start)
    while (pos < 3) {
        str += contents.at(pos++);
    }

    if (str != "RSP") {
        Log(Log::Warning, "Not a valid .lvl file!");
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
        Log(Log::Warning, ".rsp File Header doesn't contain \"_H_E_\" at the correct spot!");
        return;
    }

    str.clear();

    while (pos < contents.size()) {
        while (contents.at(pos) != ':') {
            str += contents.at(pos);
            pos++;
        }

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

    Log(Log::Info, "Successfully loaded level!");

    m_Levels[name] = l;
}

void s_Registry::RegisterSound(const std::string& name, const std::string& wavData) {
    Wave temp = LoadWaveFromMemory(".wav", (u8*)(wavData.c_str()), wavData.size());

    m_Sounds[name.c_str()] = LoadSoundFromWave(temp);
    UnloadWave(temp);
}

Texture& s_Registry::GetTexture(const std::string& name) {
    return m_Textures.at(name);
}

Level& s_Registry::GetLevel(const std::string& name) {
    return m_Levels.at(name);
}

Sound& s_Registry::GetSound(const std::string& name) {
    return m_Sounds.at(name);
}

void s_Registry::PlaySound(const std::string& name, f32 minPitch, f32 maxPitch) {
    Sound& sound = GetSound(name);
    f32 pitch = GetRandomValue(minPitch * 100, maxPitch * 100) / 100.0f;

    SetSoundPitch(sound, pitch);
    ::PlaySound(sound);
}

void s_Registry::PlaySoundFromSource(const std::string& name, Vector2 source, Vector2 dest) {
    Sound& sound = GetSound(name);
    f32 pitch = 1.0f;

    f32 distance = Vector2Distance(source, dest);
    distance = Clamp(distance, 0.0f, 100.0f);

    DrawText(TextFormat("Distance to ound is: %f", distance), 40, 130, 30, WHITE);

    // reset the pitch just in case if the sound was used with the normal PlaySound
    SetSoundPitch(sound, pitch);
    SetSoundVolume(sound, 1.0f - distance / 100.0f);
    ::PlaySound(sound);
}
