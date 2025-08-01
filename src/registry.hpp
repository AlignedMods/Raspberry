#pragma once

// The registry is used to manage all of the game's assets

#include "game.hpp"
#include "menu.hpp"

#include "raylib.h"

#include <functional>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <filesystem>

using TileID = uint8_t;

class s_Registry {
public:
    // menu
    void RegisterAllMenus();
    void AddMenu(const std::string& name, const Menu& menu);
    void AddMenuFromJSON(const std::filesystem::path& json);

    Menu& GetMenu(const std::string& name);
    bool DoesMenuExist(const std::string& name);

    // tile related stuff
	void RegisterAllTextures(); // This function will automatically look at all of the textures and load them
	void AddTile(const std::string& name, const Texture& texture); // both registers a tile AND sets the texture
    
	bool DoesTileExist(const std::string& name);
	
	void AddTileTexture(const std::string& name, const Texture& texture); // adds a texture to an existing tile
	
	Texture& GetTileTexture(const std::string& name); // retrieves a texture for a specific tile

    void RegisterAllFunctions();
    void AddFunction(const std::string& name, std::function<void(s_Game*)> func);
    void CallFunction(const std::string& name); // fully safe, i assure you
    
private:
	// map for tiles
	std::unordered_map<std::string, Texture> m_TileTextures;
    std::unordered_map<std::string, Menu> m_Menus;
    std::unordered_map<std::string, std::function<void(s_Game*)>> m_Functions;
};

inline s_Registry Registry;
