#pragma once

#include "player/player.hpp"
#include "collectables/raspberry.hpp"

#include <vector>
#include <memory>

class Player;

class Level {
public:
	void AddPlayer();
	std::shared_ptr<Player> GetPlayer() const;

	void AddTile(Tile tile);
	std::vector<Tile>& GetAllTiles();

	std::shared_ptr<Raspberry> GetRaspberry() const;
	void AddRaspberry();
	void SetFound();

	bool IsRaspberryFound();

	bool LoadLevelFromFile(const char* path);
private:
	std::shared_ptr<Player> m_Player;
	std::vector<Tile> m_Tiles;
	std::shared_ptr<Raspberry> m_Raspberry;

	bool m_RaspberryFound;
};
