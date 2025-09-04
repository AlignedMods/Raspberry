#include "registry.hpp"
#include "log.hpp"

#include "raylib.h"

#include <fstream>
#include <sstream>

void Registry::AddArchive(const std::filesystem::path& path) {
    std::string name = path.stem().string();
    Log(Log_Info, "Adding archive %s!", name.c_str());

    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        Log(Log_Error, "Failed to open file %s!", name.c_str());
        return;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string contents = ss.str();
    ss.flush();

    Archive ar;
    ar.data = contents;
    u64 i = 0;
    std::string temp;

#if 0
    while (i < 4) {
        temp += contents.at(i);
        i++;
    }

    if (temp != "RSP\n") {
        Log(Log_Error, "Archive is not a valid format!");
        return;
    }

    temp.clear();
#endif

    while (i < contents.size()) {
        Entry e;
        std::string entryName;

        while (contents.at(i) != '\n') {
            e.type += contents.at(i);
            i++;
        }
        i++;

        while (contents.at(i) != '\n') {
            entryName += contents.at(i);
            i++;
        }
        i++;

        while (contents.at(i) != '\n') {
            temp += contents.at(i);
            i++;
        }
        i++;

        e.size = std::stoi(temp);
        e.pos = i;

        temp.clear();

        ar.entries[entryName] = e;

        Log(Log_Info, "Type: %s, Name: %s, Size: %llu", e.type.c_str(), entryName.c_str(), e.size);

        i += e.size + 1;

        Log(Log_Info, "i is : %llu", i);
    }

    // archive could be a pretty heavy structure so we don't want to copy it
    m_Archives.emplace_back(std::move(ar));
}

Texture& Registry::GetTexture(const std::string& name, const std::string& archive) {
    // lazy loading as i couldn't find a better way
    if (!m_Textures.contains(name)) {
        std::string data = GetData(name, archive);

        Image i = LoadImageFromMemory(".png", (u8*)data.c_str(), data.size());
        m_Textures[name] = LoadTextureFromImage(i);
        UnloadImage(i);
    }

    return m_Textures.at(name);
}

Sound& Registry::GetSound(const std::string& name, const std::string& archive) {
    // lazy loading as i couldn't find a better way
    if (!m_Sounds.contains(name)) {
        std::string data = GetData(name, archive);
        Wave w = LoadWaveFromMemory(".wav", (u8*)data.c_str(), data.size());

        m_Sounds[name] = LoadSoundFromWave(w);
        UnloadWave(w);
    }

    return m_Sounds.at(name);
}

Level& Registry::GetLevel(const std::string& name, const std::string& archive) {
    if (!m_Levels.contains(name)) {
        std::string data = GetData(name);
        u64 i = 11;

        Level l;

        Log(Log_Info, "MYSIZE: %llu", data.size());

        while (i < data.size()) {
            Tile t;
            std::string temp;

            while (data.at(i) != ':') {
                temp += data.at(i);
                i++;
            }
            i++; // skip over the colon

            Log(Log_Info, "Temp: %s", temp.c_str());

            if (temp == "tile") {
                temp.clear();
                while (data.at(i) != '\\') {
                    temp += data.at(i);
                    i++;
                }
                i++;

                t.type = temp;
                Log(Log_Info, "texture is %s", temp.c_str());
                t.texture = GetTexture(temp);

                temp.clear();

                i32 x = static_cast<i32>((i8)data.at(i + 3) << 24 |
                                         (i8)data.at(i + 2) << 16 |
                                         (i8)data.at(i + 1) << 8 |
                                         (i8)data.at(i + 0));

                i += 4;

                i32 y = static_cast<i32>((i8)data.at(i + 3) << 24 |
                                         (i8)data.at(i + 2) << 16 |
                                         (i8)data.at(i + 1) << 8 |
                                         (i8)data.at(i + 0));

                i += 4;

                t.pos = {static_cast<float>(x), static_cast<float>(y)};

                Log(Log_Info, "Adding tile %d, %d", x, y);

                l.tiles.push_back(t);
            }

            temp.clear();

            m_Levels[name] = l;
        }
    }

    return m_Levels.at(name);
}

std::string Registry::GetData(const std::string& name, const std::string& archive) {
    std::string temp;

    if (archive == "$$$AUTO$$$") {
        for (auto& ar : m_Archives) {
            if (ar.entries.contains(name)) {
                temp.clear(); // in case if we have more than one archive

                u64 pos = ar.entries.at(name).pos;
                u64 size = ar.entries.at(name).size;
                std::string& data = ar.data;

                Log(Log_Info, "Getting data with AUTO mode, pos %llu, size %llu, string size %llu!", pos, size, data.size());

                u64 index = 0;

                while (index < size) {
                    temp += data.at(pos + index);
                    index++;
                }

                Log(Log_Info, "Finished getting data!");
            }
        }
    } else {
        for (auto& ar : m_Archives) {
            if (ar.name == archive) {
                if (ar.entries.contains(name)) {
                    u64 pos = ar.entries.at(name).pos;
                    u64 size = ar.entries.at(name).size;

                    std::string& data = ar.data;

                    u64 index = 0;

                    while (index < size) {
                        temp += data.at(pos + index);
                        index++;
                    }
                }
            }
        }
    }

    return temp;
}
