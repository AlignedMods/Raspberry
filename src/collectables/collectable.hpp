#pragma once

#include "raylib.h"
#include "tile/tile.hpp"

class Collectable {
public:
	virtual void OnTouch() {};
	virtual void OnUpdate() {};

	TilePos GetTilePos() const;
	void Draw();
protected:
	TilePos m_Pos;

	Texture2D m_Texture;
};
