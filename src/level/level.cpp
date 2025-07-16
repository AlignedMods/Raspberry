#include "level.hpp"
#include "collectables/collectable.hpp"
#include "collectables/raspberry.hpp"
#include "registry.hpp"
#include "tile/tile.hpp"
#include "core/log.hpp"

#include <cstdint>
#include <format>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>

std::map<char, std::string> tiles = {
    {0, "Stone"},
    {1, "Dirt"},
    {2, "Grass"},
    {3, "Brick"}
};

void Level::OnUpdate() {
	m_Player.OnUpdate();
	m_Collectable.OnUpdate();
}

void Level::OnRender() {
	for (auto& tile : m_Tiles) {
		tile.OnRender();
	}

	m_Player.OnRender();
	m_Collectable.OnRender();
}

void Level::Tick() {
	m_Player.UpdateTextures();
}

Player& Level::GetPlayer() {
    return m_Player;
}

void Level::AddTile(Tile tile) {
    m_Tiles.push_back(tile);
}

std::vector<Tile>& Level::GetAllTiles() {
    return m_Tiles;
}

void Level::AddCollectable() {
    m_Collectable = Raspberry();
}

Collectable& Level::GetCollectable() {
    return m_Collectable;
}

void Level::SetFound() {
    m_CollectableFound = true;
}

bool Level::IsCollectableFound() {
    return m_CollectableFound;
}

bool Level::LoadLevelFromFile(std::filesystem::path path) {
    std::ifstream file(path, std::ios::binary);

    if (!file.good()) {
        return false;
    }

    std::stringstream contentsStream;
    std::string contents;

    contentsStream << file.rdbuf();
    contents = contentsStream.str();

    file.close();

    int pos = 0;

	// a string used to check any string in the file
	std::string str;

	// check if the file is valid (contains RSP at the start)
	while (pos < 3) {
		str.append(1, contents.at(pos++));
	}

	Log(LogLevel::Info, str);

	if (str != "RSP") {
		Log(LogLevel::Warning, "Not a valid .rsp file!");
		return false;
	}

	str.clear();

	// Get the size of the name
	uint8_t nameSize = contents.at(pos++);

	// Get the actual name
	for (int i = 0; i < nameSize; i++) {
		m_Name.append(1, contents.at(pos));
		pos++;
	}

	size_t end = pos + 5;

	// Get the _H_E_ string
	while (pos < end) {
		str.append(1, contents.at(pos++));
	}

	if (str != "_H_E_") {
		Log(LogLevel::Warning, ".rsp File Header doesn't contain \"_H_E_\" at the correct spot!");
		return false;
	}

	str.clear();

    while (pos < contents.size()) {
		while (contents.at(pos) != ':') {
			str.append(1, contents.at(pos));
			pos++;
		}

		Log(LogLevel::Info, str);

		// we are parsing a tile
		if (str == "tile") {
			// skip past the ':'
			pos++;
			Log(LogLevel::Info, std::format("Value after tile is: {}", contents.at(pos)));

			uint8_t type;
			int32_t x;
			int32_t y;

			type = contents.at(pos++);

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

			AddTile(Tile(Registry.GetNameFromID(type), {x, y}));
		}

		str.clear();
    }

	m_Player.InitTextures();

    Log(LogLevel::Info, "Successfully loaded level!");

    return true;
}
