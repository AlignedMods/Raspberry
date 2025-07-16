#include "editor.hpp"
#include "registry.hpp"
#include "renderer/renderer.hpp"
#include "gui/raspGui.hpp"
#include "core/log.hpp"
#include "game.hpp"

#include "raylib.h"
#include "raymath.h"

#include <format>
#include <ios>
#include <fstream>
#include <unordered_map>

/* food is delectable sometimes */

static std::unordered_map<int, float> zooms;
static int zoomdex = 5;

Editor::Editor(Camera2D& camera) : m_Camera(camera) {
	zooms[1] = 5.0f;
	zooms[2] = 3.0f;
	zooms[3] = 2.0f;
	zooms[4] = 1.5f;
	zooms[5] = 1.0f;
	zooms[6] = 0.5f;
	zooms[7] = 0.25f;
	zooms[8] = 0.125f;
	zooms[9] = 0.065f;
}

// this function is very ugly!!
// this is due to it having to calculate the in-game position from the given mouse position
// bunch of stupid goofy looking arithmetic operations
// luckily computers were made for math :D
void Editor::PlaceTile() {
	bool canPlace = true;

	TilePosition worldPosition = {static_cast<int32_t>(std::floor( ((GetMouseX() + (m_Camera.target.x * m_Camera.zoom - m_Camera.offset.x)) / (m_Camera.zoom * 64.0f)) )),
								  static_cast<int32_t>(std::floor( ((GetMouseY() + (m_Camera.target.y * m_Camera.zoom - m_Camera.offset.y)) / (m_Camera.zoom * 64.0f)) )) };

	for (auto& tile : m_Tiles) {
		if (Vector2Equals(tile.GetPosition().RaylibVector(), worldPosition.RaylibVector())) {
			Log(LogLevel::Info, "Cannot place block here!");
			canPlace = false;
		}
	}

	if (canPlace) {
		m_Tiles.push_back(Tile(m_CurrentTile, worldPosition));
		Log(LogLevel::Info, "place");
	}
}

// this function is also pretty ugly but it works!
// yes i am doing m_Tiles.begin() + i
// shut up it works
void Editor::DestroyTile() {
	TilePosition worldPosition = {static_cast<int32_t>(std::floor( ((GetMouseX() + (m_Camera.target.x * m_Camera.zoom - m_Camera.offset.x)) / (m_Camera.zoom * 64.0f)) )),
								  static_cast<int32_t>(std::floor( ((GetMouseY() + (m_Camera.target.y * m_Camera.zoom - m_Camera.offset.y)) / (m_Camera.zoom * 64.0f)) )) };

	std::vector<Tile>::iterator it;

	for (size_t i = 0; i < m_Tiles.size(); i++) {
		if (Vector2Equals(m_Tiles.at(i).GetPosition().RaylibVector(), worldPosition.RaylibVector())) {
			m_Tiles.erase(m_Tiles.begin() + i);
		}
	}
}

void Editor::OnUpdate() {
	// Switching modes
	if (IsKeyPressed(KEY_ONE)) {
		m_Mode = EditorModes::Pan;
	}

	if (IsKeyPressed(KEY_TWO)) {
		m_Mode = EditorModes::Build;
	}

	if (IsKeyPressed(KEY_THREE)) {
		m_Mode = EditorModes::Destroy;
	}

	// zooming
	if (GetMouseWheelMove() > 0.0f) {
		zoomdex = std::max(zoomdex - 1, 1);
	}

	if (GetMouseWheelMove() < 0.0f) {
		zoomdex = std::min(zoomdex + 1, 9);
	}

	m_Camera.zoom = zooms.at(zoomdex);

	// Switching states
	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		m_State = EditorStates::Drag;
	} else if (IsKeyUp(KEY_LEFT_SHIFT)) {
		m_State = EditorStates::Click;
	}

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F)) {
		m_Searching = true;
		m_Search.clear();
	}

	// Panning (dragging doesn't affect anything)
	if (m_Mode == EditorModes::Pan) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			// make the camera move at the same speed even with different zoom values
			m_Camera.target = Vector2Add(m_Camera.target, Vector2Scale(GetMouseDelta(), Game.deltaTime / m_Camera.zoom * -900));
		}
	}

	// building (dragging DOES affect)
	if (m_Mode == EditorModes::Build) {
		// currently dragging
		if (m_State == EditorStates::Drag) {
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				PlaceTile();
			}
		} 

		if (m_State == EditorStates::Click) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				PlaceTile();
			}
		}
	}

	// destroying :D
	if (m_Mode == EditorModes::Destroy) {
		// currently dragging
		if (m_State == EditorStates::Drag) {
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				DestroyTile();
			}
		} 

		if (m_State == EditorStates::Click) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				DestroyTile();
			}
		}
	}

	if (m_Searching) {
		if (RaspGui::TextInput({0.3f, 0.3f, 0.4f, 0.2f}, m_Search)) m_Searching = false;
	} else if (!m_Search.empty() ){
		if (Registry.DoesTileExist(m_Search)) {
			m_CurrentTile = m_Search;
		} else {
			Log(LogLevel::Error, "Tile does not exist!");
		}

		m_Search.clear();
	}

	if (RaspGui::Button({0.7f, 0.8f, 0.2f, 0.1f}, "Export")) {
		std::ofstream file("hi.rsp", std::ios::binary);

		std::string name = "hi";

		file << "RSP";
		file << static_cast<uint8_t>(name.length());
		file << name;
		file << "_H_E_";

		for (auto& tile : m_Tiles) {
			file << "tile:";
			file << Registry.GetIDFromName(tile.GetType());

			TilePosition pos = tile.GetPosition();

			Log(LogLevel::Info, std::format("X position: {}", pos.x));
			Log(LogLevel::Info, std::format("Y position: {}", pos.y));

			// X position
			file << static_cast<int8_t>(pos.x & 0xff);
			file << static_cast<int8_t>((pos.x >> 8) & 0xff);
			file << static_cast<int8_t>((pos.x >> 16) & 0xff);
			file << static_cast<int8_t>((pos.x >> 24) & 0xff);

			// Y position
			file << static_cast<int8_t>(pos.y & 0xff);
			file << static_cast<int8_t>((pos.y >> 8) & 0xff);
			file << static_cast<int8_t>((pos.y >> 16) & 0xff);
			file << static_cast<int8_t>((pos.y >> 24) & 0xff);
		}
	}
}

void Editor::OnRender() {
	BeginMode2D(m_Camera);

	for (auto& tile : m_Tiles) {
		tile.OnRender();
	}

	EndMode2D();
}
