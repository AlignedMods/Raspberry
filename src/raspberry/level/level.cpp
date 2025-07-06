#include "level.hpp"
#include "raspberry/collectables/collectable.hpp"
#include "raspberry/collectables/raspberry.hpp"
#include "raspberry/tile/tile.hpp"
#include "shared/log.hpp"

#include <cstdint>
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

void Level::AddPlayer() {
    m_Player = std::make_shared<Player>();
}

std::shared_ptr<Player> Level::GetPlayer() {
    return m_Player;
}

void Level::AddTile(std::shared_ptr<Tile> tile) {
    m_Tiles.push_back(tile);
}

std::vector<std::shared_ptr<Tile>>& Level::GetAllTiles() {
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

    while (pos < contents.size()) {

        // (tiles.at(contents.at(pos)),
        //{(float)contents.at(pos + 3), (float)contents.at(pos + 4)})

        //Debug(pos);

        std::shared_ptr<Tile> tile = std::make_shared<Tile>(Tile(tiles.at(contents.at(pos)),
																 {(uint16_t)(contents.at(pos + 3)),
																  (uint16_t)(contents.at(pos + 4))}));

        AddTile(tile);

        pos += 8;
    }

    Debug("Sucssesfully loaded level!");

    return true;
}
