#pragma once

#include "tile/tile.hpp"

#include "raylib.h"

#include <vector>

enum class EditorModes {
	Pan,
	Build,
	Select,
	Destroy
};

enum class EditorStates {
	Click,
	Drag
};

class Editor {
public:
	Editor(Camera2D& camera);

	void OnRender();
	void OnUpdate();

	void PlaceTile();

private:
	std::vector<Tile> m_Tiles;
	std::string m_CurrentTile = "Dirt";
	Tile* m_Selection = nullptr;
	EditorModes m_Mode = EditorModes::Build;
	EditorStates m_State = EditorStates::Click;

	bool m_Searching = false;
	std::string m_Search;

	Camera2D& m_Camera;
};
