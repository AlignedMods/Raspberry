#pragma once

#include "blueberry/tile/tile.hpp"

#include "raylib.h"

#include <vector>
#include <filesystem>

class Editor {
public:
	Editor(Camera2D& camera);
	Editor(Camera2D& camera, std::filesystem::path level);

	void Update();
	void Draw();

	void DrawMenu();

	void ExportLevel();

private:
	int m_SelectedTile;
	char* m_LoadedData;
	std::vector<Tile> m_Tiles;

	std::string m_Name;

	Camera2D& m_Camera;

	bool m_IsCurrentDrawn = false;
};
