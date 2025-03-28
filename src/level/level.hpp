#pragma once

#include <collectables/diamond.hpp>
#include <collectables/raspberry.hpp>
#include <pch.hpp>

#include "collectables/collectable.hpp"
#include "entity/player.hpp"
#include "tile/tile.hpp"

class Player;

class Level {
 public:
  void AddPlayer();
  std::shared_ptr<Player> GetPlayer() const;

  void AddTile(const Tile& tile);
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
