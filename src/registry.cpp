#include "registry.hpp"
#include "core/log.hpp"
#include "core/types.hpp"

#include "game.hpp"
#include "nlohmann/json.hpp"

#include <filesystem>
#include <fstream>
#include <functional>

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
            std::cout << "\nENTRY: " << entries << " {\n";

            auto& entry = data.at(entries);

            if (entry.contains("type")) {
                std::cout << entry.at("type") << '\n';
                if (entry.at("type") == "button") {
                    std::array<i32, 4> dim = entry.at("bounds");

                    menu.Buttons.push_back({ {(f32)dim[0], (f32)dim[1], (f32)dim[2], (f32)dim[3]}, entry.at("text"), entry.at("on-click") });
                }
            }

            std::cout << "}\n";
            std::cout << "\n------------------\n";
        }

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
