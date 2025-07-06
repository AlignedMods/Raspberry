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
#include "blueberry/builder.hpp"
#include "blueberry/renderer/renderer.hpp"
#include "shared/types.hpp"
#include "blueberry/tile/tile.hpp"
#include "shared/log.hpp"

#include "raylib.h"
#include "raygui.h"
#include "raymath.h"
#include "rlgl.h"

#include <cstdint>
#include <stdexcept>
#include <map>
#include <fstream>

std::map<char, std::string> tiles = {
    {0, "Stone"},
    {1, "Dirt"},
    {2, "Grass"},
    {3, "Brick"},
    {4, "Grasspatch"}
};

std::map<std::string, char> tilesToBinary = {
    {"Stone", 0},
    {"Dirt", 1},
    {"Grass", 2},
    {"Brick", 3},
    {"Grasspatch", 4}
};

Editor::Editor(Camera2D& camera)  : m_Camera(camera) {
	m_SelectedTile = 0;
}

Editor::Editor(Camera2D& camera, std::filesystem::path path)  : m_Camera(camera) {
	m_SelectedTile = 0;

    std::ifstream file(path, std::ios::binary);

    std::stringstream contentsStream;
    std::string contents;

    contentsStream << file.rdbuf();
    contents = contentsStream.str();

    file.close();

    int pos = 0;

    while (pos < contents.size()) {

        // (tiles.at(contents.at(pos)),
        //{(float)contents.at(pos + 3), (float)contents.at(pos + 4)})

        //Debug(contents.size());

        try {
            //m_Tiles.push_back(Tile(tiles.at(contents.at(pos)), {static_cast<float>(contents.at(pos + 3)), static_cast<float>(contents.at(pos + 4))}));
        } catch (std::out_of_range) {
            std::stringstream msg;

            msg << "Something went wrong at position: " << pos << ", which has a value of: " << contents.at(pos) << "!";

            Debug(msg.str());
        }

        pos += 8;
    }

    Debug("Sucssesfully loaded level!");
}

void Editor::Update() {
	if (IsKeyDown(KEY_W)) {
		m_Camera.target = {m_Camera.target.x, m_Camera.target.y - 10.0f};
	} else if (IsKeyDown(KEY_S)) {
		m_Camera.target = {m_Camera.target.x, m_Camera.target.y + 10.0f};
	}
    
    if (IsKeyDown(KEY_A)) {
		m_Camera.target = {m_Camera.target.x - 10.0f, m_Camera.target.y};
	} else if (IsKeyDown(KEY_D)) {
		m_Camera.target = {m_Camera.target.x + 10.0f, m_Camera.target.y};
	}

	if (!m_IsCurrentDrawn) {
		//m_Tiles.push_back(Tile(tiles.at(m_SelectedTile), {18.5, 7}));
		m_IsCurrentDrawn = true;
	}

	if (GetMouseWheelMove() > 0) {
		if (IsKeyDown(KEY_LEFT_CONTROL)) {
			m_Camera.zoom += 0.2f;
		} else {
			m_SelectedTile = std::min(m_SelectedTile + 1, static_cast<int>(tiles.size()) - 1);

            //Debug(m_SelectedTile);
		}

		//m_Tiles.push_back(Tile(tiles.at(m_SelectedTile), {18.5, 7}, TileArgs::NO_CAMERA));
	} else if (GetMouseWheelMove() < 0) {
		if (IsKeyDown(KEY_LEFT_CONTROL)) {
			m_Camera.zoom -= 0.2f;
		} else {
			if (m_SelectedTile == 0) {
				m_SelectedTile = 4;
			} else {
				m_SelectedTile--;
			}
		}
		//m_Tiles.push_back(Tile(tiles.at(m_SelectedTile), {18.5, 7}, TileArgs::NO_CAMERA));
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		if (!CheckCollisionPointRec({(float)(GetMouseX()), (float)(GetMouseY())}, {17 * 64, 0, 4 * 64, 12 * 64})) {
            bool canPlace = true;

			for (Tile tile : m_Tiles) {
				if (Vector2Equals(tile.GetPosition().RaylibVector(),
					  {std::floor((GetMouseX() + m_Camera.target.x) / 64 / m_Camera.zoom), std::floor((GetMouseY() + m_Camera.target.y) / 64 / m_Camera.zoom)})) {
					Debug("no place block");
                    canPlace = false;
                }
			}

            if (canPlace) {
                Debug(std::floor((GetMouseX() + m_Camera.target.x) / 64 / m_Camera.zoom));
                m_Tiles.push_back(Tile(tiles.at(m_SelectedTile),
					{(uint16_t)std::floor((GetMouseX() + m_Camera.target.x) / 64 / m_Camera.zoom),
					 (uint16_t)std::floor((GetMouseY() + m_Camera.target.y) / 64 / m_Camera.zoom)}));
            }

            canPlace = true;
		}
	}
}

void Editor::Draw() {
	for (Tile tile : m_Tiles) {
		//if (tile.GetTileArgs() != TileArgs::NO_CAMERA) {
			tile.OnRender();
		//}
	}
}

void Editor::ExportLevel() {
	std::ofstream file("hi.lvl", std::ios::binary);

	// Header
	
	m_Name = "death below";

	file << "RSP";
	file << static_cast<uint8_t>(m_Name.size());
	file << m_Name.c_str();
	file << "_H_E_";

	// for exporting the level
	for (Tile tile : m_Tiles) {
		//if (tile.GetTileArgs() != TileArgs::NO_CAMERA) {
			// puts the tiletype stored as a char in the file
			file << tilesToBinary.at(tile.GetType());
			// tS stands for tile separator and it separated the tiletype and position
			file << "tS";
			// puts the position in the file
			
			file << static_cast<char>(tile.GetPosition().x);
			file << static_cast<char>(tile.GetPosition().y);
			// rSP stands for raspberry separator and is used to separate tiles
			file << "rSP";
		}
	//}

	file.close();
}

void Editor::DrawMenu() {
	// Set the text size to be a little smaller
	GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

	GuiPanel({17 * 64, 0, 4 * 64, 12 * 64}, "Info");

	for (Tile tile : m_Tiles) {
		//if (tile.GetTileArgs() == TileArgs::NO_CAMERA) {
			tile.OnRender();
		//}
	}

	Renderer.RenderText("Current Tile: ", 8.5 * 64, 5 * 64);
	if (Renderer.RenderButton({19 * 64, 10.9 * 64, 120, 64}, "Export")) {
		ExportLevel();
	}
}
