#pragma once

#include "types.hpp"

#include "raylib.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

// An entry in an RSP file looks like this
// --------------------------
// type: ex. font
// name: ex. my-cool-font (declared in map of entries)
// size: ex. 50000 (in bytes)
// --------------------------
struct Entry {
    std::string type; // type of data
    u64 size;         // size of data
    u64 pos;          // position in file where the data begins
};

struct Archive {
    std::string name;
    std::string data;
    std::unordered_map<std::string, Entry> entries;
};

struct Tile {
    std::string type;
    Vector2 pos;
    Texture texture;
};

struct Level {
    std::vector<Tile> tiles;
};

class Registry {
public:
    void AddArchive(const std::filesystem::path& path);

    Texture& GetTexture(const std::string& name, const std::string& archive = "$$$AUTO$$$");
    Sound& GetSound(const std::string& name, const std::string& archive = "$$$AUTO$$$");
    Level& GetLevel(const std::string& name, const std::string& archive = "$$$AUTO$$$");

    // gets the data at the specified name
    std::string GetData(const std::string& name, const std::string& archive = "$$$AUTO$$$");

private:
    std::vector<Archive> m_Archives;
    std::unordered_map<std::string, Texture> m_Textures;
    std::unordered_map<std::string, Sound> m_Sounds;
    std::unordered_map<std::string, Level> m_Levels;
};
