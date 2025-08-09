#pragma once

#include "core/types.hpp"
#include "entity/mob.hpp"
#include "entity/player.hpp"
#include "tile/tile.hpp"

#include <vector>

class Level {
public:
    void OnUpdate();
    void OnRender();
    void Tick();

    Player& GetPlayer();

    void AddTile(Tile tile);
    std::vector<Tile>& GetAllTiles();

    void SetFound();

    bool IsCollectableFound();

    void SetName(const std::string& name);
    std::string& GetName();

private:
    std::vector<Tile> m_Tiles;

    std::string m_Name;

    Player m_Player{};

    f32 m_Brightness = 1.0f;
    Shader m_Shader;

    std::vector<Mob> m_Mobs;

    bool m_CollectableFound;
};
