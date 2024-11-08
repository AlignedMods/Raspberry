/*
* I know this is really undocumented and i will try to add comments soon ^_^
*
*
*
*
*
*
*
*
*
*
*
*
* no actually though i'm starting to get confused here
*/

#include "editor.hpp"
#include "builder/builder.hpp"
#include "raylib.h"
#include "raygui.h"
#include "tile/tile.hpp"

#include "pch.hpp"

Editor::Editor(Camera2D& camera)  : m_Camera(camera) {
	m_SelectedTile = 0;
}

void Editor::Update() {
	if (IsKeyDown(KEY_W)) {
		m_Camera.target = {m_Camera.target.x, m_Camera.target.y - 10.0f};
	} else if (IsKeyDown(KEY_S)) {
		m_Camera.target = {m_Camera.target.x, m_Camera.target.y + 10.0f};
	} else if (IsKeyDown(KEY_A)) {
		m_Camera.target = {m_Camera.target.x - 10.0f, m_Camera.target.y};
	} else if (IsKeyDown(KEY_D)) {
		m_Camera.target = {m_Camera.target.x + 10.0f, m_Camera.target.y};
	}

	if (!m_IsCurrentDrawn) {
		m_Tiles.push_back(Tile((TileType)(m_SelectedTile + 1), {18.5, 7}, TileArgs::NO_CAMERA));
		m_IsCurrentDrawn = true;
	}

	if (GetMouseWheelMove() > 0) {
		if (IsKeyDown(KEY_LEFT_CONTROL)) {
			m_Camera.zoom += 0.2f;
		} else {
			std::cout << "Scrolling";
			if (m_SelectedTile == 3) {
				m_SelectedTile = 0;
			} else {
				m_SelectedTile++;
			}
		}
		m_Tiles.push_back(Tile((TileType)(m_SelectedTile + 1), {18.5, 7}, TileArgs::NO_CAMERA));
	} else if (GetMouseWheelMove() < 0) {
		if (IsKeyDown(KEY_LEFT_CONTROL)) {
			m_Camera.zoom -= 0.2f;
		} else {
			if (m_SelectedTile == 0) {
				m_SelectedTile = 3;
			} else {
				m_SelectedTile--;
			}
		}
		m_Tiles.push_back(Tile((TileType)(m_SelectedTile + 1), {18.5, 7}, TileArgs::NO_CAMERA));
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (!CheckCollisionPointRec({(float)(GetMouseX()), (float)(GetMouseY())}, {17 * 64, 0, 4 * 64, 12 * 64})) {
			m_Tiles.push_back(Tile((TileType)(m_SelectedTile + 1), {std::floor((GetMouseX() + m_Camera.target.x) / 64 / m_Camera.zoom), std::floor((GetMouseY() + m_Camera.target.y) / 64 / m_Camera.zoom)}));
		}
	}
}

void Editor::Draw() {
	for (Tile tile : m_Tiles) {
		if (tile.GetTileArgs() != TileArgs::NO_CAMERA) {
			tile.Draw();
		}
	}
}

void Editor::DrawMenu() {
	// Set the text size to be a little smaller
	GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

	GuiPanel({17 * 64, 0, 4 * 64, 12 * 64}, "Info");

	for (Tile tile : m_Tiles) {
		if (tile.GetTileArgs() == TileArgs::NO_CAMERA) {
			tile.Draw();
		}
	}

	Builder::GetRenderer()->RenderText("Current Tile: ", 8.5 * 64, 5 * 64);
	if (Builder::GetRenderer()->RenderButton({19 * 64, 10.9 * 64, 120, 64}, "Export")) {
		std::ofstream file("hi.lvl", std::ios::binary);

		// for exporting the level
		for (Tile tile : m_Tiles) {
			if (tile.GetTileArgs() != TileArgs::NO_CAMERA) {
				// puts the tiletype stored as a char in the file
				file << (char)tile.GetTileType();
				// tS stands for tile separator and it separated the tiletype and position
				file << "tS";
				// puts the position in the file
				file << (char)tile.GetPos().x;
				file << (char)tile.GetPos().y;
				// rSP stands for raspberry separator and is used to separate tiles
				file << "rSP";
			}
		}

		file.close();
	}
}
