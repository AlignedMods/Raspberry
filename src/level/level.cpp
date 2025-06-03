#include "level.hpp"
#include <memory>
#include "collectables/collectable.hpp"
#include "collectables/raspberry.hpp"
#include "core/core.hpp"

#include "tile/tile.hpp"

#include "pch.hpp"

std::map<char, std::string> tiles = {
    {0, "Stone"},
    {1, "Dirt"},
    {2, "Grass"},
    {3, "Brick"}
};

void Level::AddPlayer() {
    m_Player = std::make_shared<Player>();

    m_Instances.push_back(m_Player);
}

std::shared_ptr<Player> Level::GetPlayer() {
    return m_Player;
}

void Level::AddTile(std::shared_ptr<Tile> tile) {
    m_Tiles.push_back(tile);
    m_Instances.push_back(tile);
}

std::vector<std::shared_ptr<Tile>>& Level::GetAllTiles() {
    return m_Tiles;
}

std::vector<std::shared_ptr<Instance>>& Level::GetInstances() {
    return m_Instances;
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

        std::shared_ptr<Tile> tile = std::make_shared<Tile>(Tile(tiles.at(contents.at(pos)), {static_cast<float>(contents.at(pos + 3)), static_cast<float>(contents.at(pos + 4))}));

        AddTile(tile);

        pos += 8;
    }

    Debug("Sucssesfully loaded level!");

    return true;
}
