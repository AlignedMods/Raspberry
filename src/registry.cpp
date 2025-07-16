#include "registry.hpp"

void s_Registry::RegisterTile(const std::string& name) {
	m_TileNameToID[name] = m_NumberOfTiles;
	m_TileIDToName[m_NumberOfTiles] = name;

	m_NumberOfTiles++;
}

std::string& s_Registry::GetNameFromID(TileID id) {
	return m_TileIDToName.at(id);
}

TileID s_Registry::GetIDFromName(const std::string& name) {
	return m_TileNameToID.at(name);
}

void s_Registry::AddTileTexture(const std::string& name, const Texture& texture) {
	// will throw an exception if the tile isn't already registered
	m_TileTextures[m_TileNameToID.at(name)] = texture;
}

Texture& s_Registry::GetTileTexture(const std::string& name) {
	return m_TileTextures.at(m_TileNameToID.at(name));
}

Texture& s_Registry::GetTileTexture(TileID id) {
	return m_TileTextures.at(id);
}

void s_Registry::AddTile(const std::string& name, const Texture& texture) {
	RegisterTile(name);
	AddTileTexture(name, texture);
}

bool s_Registry::DoesTileExist(const std::string& name) {
	if (!m_TileNameToID.contains(name)) {
		return false;
	}

	return m_TileTextures.contains(m_TileNameToID.at(name));
}
