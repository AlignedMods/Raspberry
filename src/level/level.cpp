#include "level.hpp"
#include "collectables/raspberry.hpp"
#include "tile/tile.hpp"

#include "pch.hpp"

void Level::AddPlayer() {
	m_Player = std::make_shared<Player>();
}

std::shared_ptr<Player> Level::GetPlayer() const {
	return m_Player;
}

void Level::AddTile(Tile tile) {
	m_Tiles.push_back(tile);		
}

std::vector<Tile>& Level::GetAllTiles() {
	return m_Tiles;
}

void Level::AddRaspberry() {
	m_Raspberry = std::make_shared<Raspberry>();
}

std::shared_ptr<Raspberry> Level::GetRaspberry() const {
	return m_Raspberry;
}

void Level::SetFound() {
	m_RaspberryFound = true;
}

bool Level::IsRaspberryFound() {
	return m_RaspberryFound;
}

bool Level::LoadLevelFromFile(const char* path) {
	std::ifstream file(path);

	std::stringstream contentsStream;
	std::string contents;

	contentsStream << file.rdbuf();
	contents = contentsStream.str();

	file.close();

	int pos = 0;

	while (pos < contents.size()) {
		AddTile(Tile((TileType)contents.at(pos), {(float)contents.at(pos + 3), (float)contents.at(pos + 4)}));

		pos += 8;
	}

	return true;
}
