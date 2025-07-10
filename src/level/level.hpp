#pragma once

#include "collectables/diamond.hpp"
#include "collectables/raspberry.hpp"
#include "collectables/collectable.hpp"
#include "entity/player.hpp"
#include "tile/tile.hpp"

#include "box2d/box2d.h"

#include <memory>
#include <vector>
#include <filesystem>

class Level {
public:
	void OnUpdate();
	void OnRender();
	void Tick();

    Player& GetPlayer();

    void AddTile(Tile tile);
    std::vector<Tile>& GetAllTiles();

    void AddCollectable();
    Collectable& GetCollectable();

    void SetFound();

    bool IsCollectableFound();

    bool LoadLevelFromFile(std::filesystem::path path);

private:
    std::vector<Tile> m_Tiles;

	std::string m_Name;

    Collectable m_Collectable;
    Player m_Player{};

    bool m_CollectableFound;
};
