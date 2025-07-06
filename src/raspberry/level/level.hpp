#pragma once

#include "raspberry/collectables/diamond.hpp"
#include "raspberry/collectables/raspberry.hpp"
#include "raspberry/collectables/collectable.hpp"
#include "raspberry/entity/player.hpp"
#include "raspberry/tile/tile.hpp"

#include <memory>
#include <vector>
#include <filesystem>

class Player;

class Level {
    public:
    void AddPlayer();
    std::shared_ptr<Player> GetPlayer();

    void AddTile(std::shared_ptr<Tile> tile);
    std::vector<std::shared_ptr<Tile>>& GetAllTiles();

    void AddCollectable();
    Collectable& GetCollectable();

    void SetFound();

    bool IsCollectableFound();

    bool LoadLevelFromFile(std::filesystem::path path);

    private:
    std::vector<std::shared_ptr<Tile>> m_Tiles;

    Collectable m_Collectable;
    std::shared_ptr<Player> m_Player;

    bool m_CollectableFound;
};
