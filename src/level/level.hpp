#pragma once

#include "collectables/collectable.hpp"
#include "player/player.hpp"
#include "tile/tile.hpp"
#include <collectables/diamond.hpp>
#include <collectables/raspberry.hpp>

#include <pch.hpp>

class Player;

class Level {
public:
    void AddPlayer();
    std::shared_ptr<Player> GetPlayer() const;

    void AddTile(Tile tile);
    std::vector<Tile>& GetAllTiles();

    void AddCollectable();
    Collectable& GetCollectable();

    void SetFound();

    bool IsCollectableFound();

    bool LoadLevelFromFile(const char* path);

private:
    std::vector<Tile> m_Tiles;

    Collectable m_Collectable;
    std::shared_ptr<Player> m_Player;

    bool m_CollectableFound;
};
