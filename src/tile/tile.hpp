#pragma once

#include "raylib.h"

enum class TileType {
	AIR, DIRT, STONE, GRASS, BRICK
};

enum class TileArgs {
	NORMAL, NO_CAMERA
};

typedef struct TilePos {
	float x;
	float y;
} TilePos;

class Tile {
public:
	Tile(TileType tileType, TilePos tilePos);
	Tile(TileType tileType, TilePos tilePos, TileArgs tileArgs);

	void Draw();
	float GetX();
	float GetY();

	Texture2D GetTexture();

	TilePos GetPos();
	TileType GetTileType();

	float GetSize();

	TileArgs GetTileArgs();
private:
	TileType m_TileType;
	TilePos m_TilePos;
	TilePos m_TileSize;

	TileArgs m_TileArgs;

	Texture2D m_Texture;

	float m_Size;

	float m_X;
	float m_Y;
};
