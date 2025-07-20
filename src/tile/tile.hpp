#pragma once

#include "core/types.hpp"

#include "raylib.h"

#include <string>

class Tile {
public:
	Tile() = default;
	Tile(const std::string& type, const TilePosition& position);

	void OnRender();
	void OnRender(uint32_t flag);

	TilePosition& GetPosition();
	std::string& GetType();

private:
	TilePosition m_Position;
	std::string m_Type;

	Texture m_Texture;
};
