#pragma once

#include "level/level.hpp"

#include "raylib.h"

#include <filesystem>
#include <unordered_map>

class s_Registry {
public:
    void RegisterArchive(const std::filesystem::path& archive);

    void RegisterTexture(const std::string& name, const std::string& pngData, const u64 size);
    void RegisterLevel(const std::string& name, const std::string& lvlData);
    void RegisterSound(const std::string& name, const std::string& wavData);

    Texture& GetTexture(const std::string& name);
    Level& GetLevel(const std::string& name);
    Sound& GetSound(const std::string& name);

    // utility functions (not neccesary)
    void PlaySound(const std::string& name, f32 minPitch, f32 maxPitch);
    void PlaySoundFromSource(const std::string& name, Vector2 source, Vector2 dest); // calculates the volume of the sound based on the distance

private:
    std::unordered_map<std::string, Texture> m_Textures;
    std::unordered_map<std::string, Level> m_Levels;
    std::unordered_map<std::string, Sound> m_Sounds;
};

extern s_Registry Registry;
