#pragma once

#include "raylib.h"
#include "tile/tile.hpp"
#include <vector>

class Editor {
public:
	Editor(Camera2D& camera);
	Editor(Camera2D& camera, std::filesystem::path level);

	void Update();
	void Draw();

	void DrawMenu();

private:
	int m_SelectedTile;
	char* m_LoadedData;
	std::vector<Tile> m_Tiles;

	Camera2D& m_Camera;

	bool m_IsCurrentDrawn = false;
};
