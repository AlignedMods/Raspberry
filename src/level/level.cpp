#include "level.hpp"
#include "collectables/collectable.hpp"
#include "collectables/raspberry.hpp"
#include "core/core.hpp"
#include "tile/tile.hpp"

#include "pch.hpp"

void Level::AddPlayer()
{
    m_Player = std::make_shared<Player>();
}

std::shared_ptr<Player> Level::GetPlayer() const
{
    return m_Player;
}

void Level::AddTile(Tile tile)
{
    m_Tiles.push_back(tile);
}

std::vector<Tile>& Level::GetAllTiles()
{
    return m_Tiles;
}

void Level::AddCollectable()
{
    m_Collectable = Raspberry();
}

Collectable& Level::GetCollectable()
{
    return m_Collectable;
}

void Level::SetFound()
{
    m_CollectableFound = true;
}

bool Level::IsCollectableFound()
{
    return m_CollectableFound;
}

bool Level::LoadLevelFromFile(const char* path)
{
    std::ifstream file(path);

    std::stringstream contentsStream;
    std::string contents;

    contentsStream << file.rdbuf();
    contents = contentsStream.str();

    file.close();

    int pos = 0;

    while (pos < contents.size()) {
        AddTile(Tile((TileType)contents.at(pos),
            { (float)contents.at(pos + 3), (float)contents.at(pos + 4) }));

        pos += 8;
    }

    Debug("Sucssesfully loaded level!");

    return true;
}
