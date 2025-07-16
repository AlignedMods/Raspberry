#pragma once

// The registry is used to manage all of the game's assets

#include "raylib.h"

#include <unordered_map>
#include <cstdint>
#include <string>

using TileID = uint8_t;

class s_Registry {
public:
	void AddTile(const std::string& name, const Texture& texture); // both registers a tile AND sets the texture
	
	std::string& GetNameFromID(TileID id);
	TileID GetIDFromName(const std::string& name);
	
	void RegisterTile(const std::string& name); // registers a tile with the given name
	void AddTileTexture(const std::string& name, const Texture& texture); // adds a texture to an existing tile
	
	Texture& GetTileTexture(const std::string& name); // retrieves a texture for a specific tile
	Texture& GetTileTexture(TileID id); // retrieves a texture for a specific tile
	
	bool DoesTileExist(const std::string& name);
private:
	// maps for tiles
	std::unordered_map<TileID, Texture> m_TileTextures;
	std::unordered_map<std::string, TileID> m_TileNameToID;
	std::unordered_map<TileID, std::string> m_TileIDToName;
	TileID m_NumberOfTiles = 0;
};

inline s_Registry Registry;
