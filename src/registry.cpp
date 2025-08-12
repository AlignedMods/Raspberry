#include "registry.hpp"
#include "core/log.hpp"
#include "core/types.hpp"
#include "game.hpp"

#include "nlohmann/json.hpp"
#include "raylib.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <utility>

GuiInfo GetInfo(const nlohmann::json& entry) {
    std::array<i32, 4> bounds;
    std::pair<std::string, std::string> anchor;
    bool active;

    // you technically do not need to specify any of this info
    if (!entry.contains("offset")) {
        bounds[0] = 0;
        bounds[1] = 0;
    } else {
        bounds[0] = entry.at("offset")[0];
        bounds[1] = entry.at("offset")[1];
    }

    if (!entry.contains("size")) {
        bounds[2] = 200;
        bounds[3] = 50;
    } else {
        bounds[2] = entry.at("size")[0];
        bounds[3] = entry.at("size")[1];
    }

    if (!entry.contains("anchor")) {
        anchor.first = "middle";
        anchor.second = "middle";
    } else {
        anchor.first = entry.at("anchor")[0];
        anchor.second = entry.at("anchor")[1];
    }

    if (!entry.contains("active")) {
        active = true;
    } else {
        if (entry.at("active") == "true") { active = true; }
        else if (entry.at("active") == "false") { active = false; }
    }

    return { {(f32)bounds[0], (f32)bounds[1], (f32)bounds[2], (f32)bounds[3]}, anchor.first, anchor.second, active };
}

std::string GetText(const nlohmann::json& entry) {
    std::string text;

    if (!entry.contains("text")) {
        text = "$NOTEXT";
    } else {
        text = entry.at("text");
    }

    return text;
}

void s_Registry::RegisterArchive(const std::filesystem::path& archive) {
    Log(LogLevel::Info, std::format("Adding archive {}", archive.string()));

    std::ifstream file(archive, std::ios::binary);

    std::stringstream contentsStream;
    std::string contents;

    std::string currentDirectory;
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
        
        currentDirectory = temp;
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

        if (currentDirectory == "textures") {
            Image im = LoadImageFromMemory(".png", (u8*)data.c_str(), size);

            AddTexture(name, LoadTextureFromImage(im));
            UnloadImage(im);
        }

        if (currentDirectory == "fonts") {
            Image im = LoadImageFromMemory(".png", (u8*)data.c_str(), data.size());
            m_Font = LoadTextureFromImage(im);

            UnloadImage(im);
        }

        if (currentDirectory == "levels") {
            AddLevelFromLvl(data);
        }

        index++;
    }
}

void s_Registry::RegisterDirectory(const std::filesystem::path& directory) {
    std::string currentDirectory;
    std::string data;

    #ifdef RDEBUG

    for (auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_directory()) {
            currentDirectory = entry.path().filename().string();
        } else {
            std::ifstream file(entry.path(), std::ios::binary);
            std::stringstream stream;

            stream << file.rdbuf();

            data = stream.str();

            if (currentDirectory == "textures") {
                AddTexture(entry.path().stem().string(), LoadTexture(entry.path().c_str()));
            }

            if (currentDirectory == "fonts") {
                Log(LogLevel::Info, entry.path().extension().string());
                if (entry.path().filename().string() == "raspberry.png") {
                    m_Font = LoadTexture(entry.path().c_str());

                    Log(LogLevel::Info, "Adding font");
                }
            }

            if (currentDirectory == "levels") {
                AddLevelFromLvl(data);
            }
        }
    }

    #else
    #error "Trying to call RegisterDirectory in release mode!"
    #endif
}

void s_Registry::AddTexture(const std::string& name, const Texture& texture) {
	// will throw an exception if the tile isn't already registered
	m_TileTextures[name] = texture;
}

Texture& s_Registry::GetTexture(const std::string& name) {
    if (!DoesTextureExist(name)) {
        Log(LogLevel::Error, std::format("Texture {} doesn't exist, or hasn't been registered yet!", name));
    }

	return m_TileTextures.at(name);
}


bool s_Registry::DoesTextureExist(const std::string& name) {
	return m_TileTextures.contains(name);
}

void s_Registry::RegisterAllFunctions() {
    // just about the most disgusting thing i have ever seen
    // jesus christ
    // i know i there is a better way but this is portable, the other version is NOT

    AddFunction("StartGame", [](){ Game.StartGameplay(); });
    AddFunction("QuitGame", [](){ Game.Quit(); });
    AddFunction("StartEditor", [](){ Game.StartEditor(); });

    // still gotta do this for the editor ones
    AddFunction("ExportLevel", [](){ Game.GetEditor()->Export(); });
}

void s_Registry::AddFunction(const std::string& name, std::function<void()> func) {
    m_Functions[name] = func;
}

void s_Registry::CallFunction(const std::string& name) {
    // very safe
    m_Functions.at(name)();
}

Texture& s_Registry::GetFont() {
    return m_Font;
}

void s_Registry::AddLevel(const std::string& name, const Level& level) {
    m_Levels[name] = level;
}

void s_Registry::AddLevelFromLvl(const std::string& lvl) {
    std::string contents;
    Level l;

    contents = lvl;

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

    AddLevel(l.GetName(), l);
}

Level& s_Registry::GetLevel(const std::string& name) {
    return m_Levels.at(name);
}
