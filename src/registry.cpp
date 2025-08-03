#include "registry.hpp"
#include "core/log.hpp"
#include "core/types.hpp"

#include "game.hpp"
#include "gui/gui.hpp"
#include "nlohmann/json.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <utility>

PosInfo GetInfo(const nlohmann::json& entry) {
    std::array<i32, 4> bounds;
    std::pair<std::string, std::string> anchor;

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

    return { {(f32)bounds[0], (f32)bounds[1], (f32)bounds[2], (f32)bounds[3]}, anchor.first, anchor.second };
}

void s_Registry::AddVariable(const std::string& name, f32 val) {
    m_Variables[name] = val;
}

void s_Registry::SetValue(const std::string& name, f32 val) {
    m_Variables[name] = val;
}

f32& s_Registry::GetValue(const std::string& name) {
    return m_Variables.at(name);
}

void s_Registry::RegisterAllMenus() {
	std::filesystem::path textures = std::filesystem::current_path() / "Assets" / "Gui";

	for (const auto& entry : std::filesystem::directory_iterator(textures)) {
		if (entry.path().extension() != "json") {
			Log(LogLevel::Info, entry.path().string());
            AddMenuFromJSON(entry);
		}
	}
}

void s_Registry::AddMenu(const std::string& name, const Menu& menu) {
    m_Menus[name] = menu;
}

void s_Registry::AddMenuFromJSON(const std::filesystem::path& json) {
    std::ifstream f(json);
    nlohmann::json data = nlohmann::json::parse(f);

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

                    menu.AddElement( Text(GetInfo(entry), text) );
                }

                if (entry.at("type") == "button") {
                    std::string text = "*blank*";

                    if (entry.contains("text")) {
                        text = entry.at("text");
                    }

                    if (!entry.contains("on-click")) {
                        Log(LogLevel::Error, "Button doesn't specify \"on-click\" property!\n" \
                                             "If you wish to not specify an action use *nop*!");

                        return;
                    }

                    menu.AddElement( Button(GetInfo(entry), text, entry.at("on-click") ) );
                }

                if (entry.at("type") == "label") {
                    std::string text = "*blank*";

                    if (entry.contains("text")) {
                        text = entry.at("text");
                    }

                    menu.AddElement( Label(GetInfo(entry), text) );
                }

                if (entry.at("type") == "hh") {
                    Log(LogLevel::Info, "checkbox!");
                    menu.AddElement( CheckBox(GetInfo(entry)) );
                }
            }
        }

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

void s_Registry::RegisterAllTextures() {
	std::filesystem::path textures = std::filesystem::current_path() / "Assets" / "Textures";

	for (const auto& entry : std::filesystem::directory_iterator(textures)) {
		if (entry.path().extension() != "png") {
			Log(LogLevel::Info, entry.path().string());
			AddTile(entry.path().stem(), LoadTexture(entry.path().string().c_str()));
		}
	}
}

void s_Registry::AddTile(const std::string& name, const Texture& texture) {
	AddTileTexture(name, texture);
}

void s_Registry::AddTileTexture(const std::string& name, const Texture& texture) {
	// will throw an exception if the tile isn't already registered
	m_TileTextures[name] = texture;
}

Texture& s_Registry::GetTileTexture(const std::string& name) {
	return m_TileTextures.at(name);
}


bool s_Registry::DoesTileExist(const std::string& name) {
	return m_TileTextures.contains(name);
}

void s_Registry::RegisterAllFunctions() {
    // just about the most disgusting thing i have ever seen
    // jesus christ

    // AddFunction("StartGame", [](){Game.StartGameplay();});

    // nevermind, found a better way!!
    AddFunction("StartGame", &s_Game::StartGameplay);
    AddFunction("QuitGame", &s_Game::Quit);
}

void s_Registry::AddFunction(const std::string& name, std::function<void(s_Game*)> func) {
    m_Functions[name] = func;
}

void s_Registry::CallFunction(const std::string& name) {
    // very safe
    m_Functions.at(name)(&Game);
}
