#include "registry.hpp"
#include "core/log.hpp"
#include "core/types.hpp"

#include "game.hpp"
#include "gui/gui.hpp"
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

        // Log(LogLevel::Info, currentDirectory);
        // Log(LogLevel::Info, name);
        // Log(LogLevel::Info, std::format("{}", size));
        // Log(LogLevel::Info, data);

        if (currentDirectory == "gui") {
            AddMenuFromJSON(data);
        }

        if (currentDirectory == "textures") {
            Image im = LoadImageFromMemory(".png", (u8*)data.c_str(), size);

            AddTexture(name, LoadTextureFromImage(im));
            UnloadImage(im);
        }

        if (currentDirectory == "fonts") {
            m_FontData = data;
        }

        if (currentDirectory == "levels") {
            AddLevelFromLvl(data);
        }

        index++;
    }
}

void s_Registry::RegisterDirectory(const std::filesystem::path& directory) {
    #ifdef RDEBUG
        
    #else
        #error "Trying to call RegisterDirectory in release mode!"
    #endif
}

void s_Registry::AddMenu(const std::string& name, const Menu& menu) {
    m_Menus[name] = menu;
}

void s_Registry::AddMenuFromJSON(const std::string& json) {
    nlohmann::json data = nlohmann::json::parse(json);

    Menu menu;
    std::string name;

    if (data.at("type") == "menu") {
        if (data.contains("name")) {
            name = data.at("name");
        }

        for (auto& entries : data.at("elements")) {
            auto& entry = data.at(entries);

            if (entry.contains("type")) {
                Log(LogLevel::Debug, entry.at("type"));

                if (entry.at("type") == "text") {
                    std::string text = "*blank*";

                    if (entry.contains("text")) {
                        text = entry.at("text");
                    }

                    menu.AddElement( Text(GetInfo(entry), text), entries );
                }

                if (entry.at("type") == "button") {
                    if (!entry.contains("on-click")) {
                        Log(LogLevel::Error, "Button doesn't specify \"on-click\" property!\n" \
                                             "If you wish to not specify an action use *nop*!");

                        return;
                    }

                    menu.AddElement( Button(GetInfo(entry), GetText(entry), entry.at("on-click")), entries );
                }

                if (entry.at("type") == "label") {
                    menu.AddElement( Label(GetInfo(entry), GetText(entry)), entries );
                }

                if (entry.at("type") == "checkbox") {
                    menu.AddElement( CheckBox(GetInfo(entry)), entries );
                }

                if (entry.at("type") == "slider") {
                    // some fallback values
                    f32 inital = 16.0f;
                    f32 min = 0.0f;
                    f32 max = 32.0f;
                    f32 step = 0.001f;

                    if (entry.contains("initial")) { inital = entry.at("inital"); }
                    if (entry.contains("min")) { min = entry.at("min"); }
                    if (entry.contains("max")) { max = entry.at("max"); }
                    if (entry.contains("step")) { step = entry.at("step"); }

                    menu.AddElement( Slider(GetInfo(entry), inital, min, max, step), entries );
                }

                if (entry.at("type") == "combobox") {
                    if (!entry.contains("options")) {
                        Log(LogLevel::Error, "ComboBox doesn't specify \"options\" property!\n" \
                                             "At least one option has to be specified in the following format:\n" \
                                             "\"options\": [ ... ]");

                        return;
                    }
                    std::vector<std::string> options = entry.at("options");

                    menu.AddElement( ComboBox(GetInfo(entry), options), entries );
                }

                if (entry.at("type") == "window") {
                    menu.AddElement( Window(GetInfo(entry), "hi"), entries );
                }

                if (entry.at("type") == "colorpicker") {
                    menu.AddElement( ColorPicker(GetInfo(entry)), entries );
                }
            }
        }

        Log(LogLevel::Info, std::format("Added menu with name: {}", name));

        menu.SetName(name);

        AddMenu(name, menu);
    }
}

Menu& s_Registry::GetMenu(const std::string& name) {
    if (DoesMenuExist(name)) {
        return m_Menus.at(name);
    }

    return m_Menus.at(name);
}

bool s_Registry::DoesMenuExist(const std::string& name) {
    return m_Menus.contains(name);
}

void s_Registry::AddTexture(const std::string& name, const Texture& texture) {
	// will throw an exception if the tile isn't already registered
	m_TileTextures[name] = texture;
}

Texture& s_Registry::GetTexture(const std::string& name) {
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

void s_Registry::AddFont(u32 size) {
    m_Fonts[size] = LoadFontFromMemory( ".ttf", (u8*)(m_FontData.c_str()), m_FontData.size(), size, nullptr, 0);
}

Font& s_Registry::GetFont(u32 size) {
    if (!m_Fonts.contains(size)) {
        AddFont(size);
    }

    return m_Fonts.at(size);
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

    if (str != "LVL") {
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

    Log(LogLevel::Info, "Successfully loaded level!");

    AddLevel(l.GetName(), l);
}

Level& s_Registry::GetLevel(const std::string& name) {
    return m_Levels.at(name);
}
