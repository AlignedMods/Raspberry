#pragma once

#include "collectables/collectable.hpp"
#include "core/types.hpp"
#include "entity/mob.hpp"
#include "entity/player.hpp"
#include "tile/tile.hpp"

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

    f32 m_Brightness = 1.0f;
    Shader m_Shader;

    std::vector<Mob> m_Mobs;

    bool m_CollectableFound;
};
