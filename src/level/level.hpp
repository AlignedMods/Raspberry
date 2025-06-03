#pragma once

#include <collectables/diamond.hpp>
#include <collectables/raspberry.hpp>
#include "collectables/collectable.hpp"
#include "entity/player.hpp"
#include "tile/tile.hpp"

#include <memory>
#include <pch.hpp>

class Player;

class Level {
    public:


    void AddPlayer();
    std::shared_ptr<Player> GetPlayer();

    void AddTile(std::shared_ptr<Tile> tile);
    std::vector<std::shared_ptr<Tile>>& GetAllTiles();
    std::vector<std::shared_ptr<Instance>>& GetInstances();

    void AddCollectable();
    Collectable& GetCollectable();

    void SetFound();

    bool IsCollectableFound();

    bool LoadLevelFromFile(std::filesystem::path path);

    private:
    std::vector<std::shared_ptr<Tile>> m_Tiles;
    std::vector<std::shared_ptr<Instance>> m_Instances;

    Collectable m_Collectable;
    std::shared_ptr<Player> m_Player;

    bool m_CollectableFound;
};
