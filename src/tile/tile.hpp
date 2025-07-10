#pragma once

#include "core/types.hpp"

#include "raylib.h"

#include <string>

class Tile {
public:
	Tile() = default;
	Tile(const std::string& type, const TilePosition& position);

	void OnRender();
	// returning this by value is intentional (returning by value is half the size lols)
	TilePosition GetPosition();
	std::string& GetType();

private:
	TilePosition m_Position;
	std::string m_Type;

	Texture m_Texture;
};
