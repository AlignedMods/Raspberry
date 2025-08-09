#pragma once

// The registry is used to manage all of the game's assets
// (well it does more but that's the gist of it)

#include "core/log.hpp"
#include "core/types.hpp"
#include "gui/gui.hpp"
#include "level/level.hpp"

#include "raylib.h"

#include <functional>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <any>

// i tried to keep templates out of this, but honestly std::any is very useful for storing variables
// and stuff, therefore any variable related stuff uses templates
class s_Registry {
public:
    void RegisterArchive(const std::filesystem::path& archive);
    void RegisterDirectory(const std::filesystem::path& directory); // should ONLY be used in debug!!

    template<typename T>
    void AddVariable(const std::string& name, T val) {
        m_Variables[name] = val;
    }

    template<typename T>
    void SetValue(const std::string& name, T val) {
        if (!m_Variables.contains(name)) {
            Log(LogLevel::Error, "Trying to set the value of a variable that doesn't exist!");
            return;
        }

        m_Variables[name] = val;
    }

    template<typename T>
    T GetValue(const std::string& name) {
        if (!m_Variables.contains(name)) {
            Log(LogLevel::Error, "Trying to access a variable that doesn't exist!");
            return T{};
        }

        try {
            return std::any_cast<T>(m_Variables.at(name));
        } catch (const std::bad_any_cast& e) {
            Log(LogLevel::Error, "Cannot cast variable to desired type!");
            return T{};
        }
    }

    bool DoesVariableExist(const std::string& name);

    // menu
    void AddMenu(const std::string& name, const Menu& menu);
    void AddMenuFromJSON(const std::string& json);

    Menu& GetMenu(const std::string& name);
    bool DoesMenuExist(const std::string& name);

    // tile related stuff
	void AddTexture(const std::string& name, const Texture& texture); // both registers a tile AND sets the texture
    
	bool DoesTextureExist(const std::string& name);
	
	Texture& GetTexture(const std::string& name); // retrieves a texture for a specific tile

    void RegisterAllFunctions();
    void AddFunction(const std::string& name, std::function<void()> func);
    void CallFunction(const std::string& name); // fully safe, i assure you

    void AddFont(u32 size);
    Font& GetFont(u32 size);

    void AddLevel(const std::string& name, const Level& level);
    void AddLevelFromLvl(const std::string& lvl);
    Level& GetLevel(const std::string& name);
    
private:
	// map for tiles
	std::unordered_map<std::string, Texture> m_TileTextures;
    std::unordered_map<u32, Font> m_Fonts;
    std::unordered_map<std::string, Menu> m_Menus;
    std::unordered_map<std::string, std::function<void()>> m_Functions;
    std::unordered_map<std::string, Level> m_Levels;

    std::string m_FontData;

    std::unordered_map<std::string, std::any> m_Variables;
};

inline s_Registry Registry;
