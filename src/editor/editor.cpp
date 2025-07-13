#include "editor.hpp"
#include "registry.hpp"
#include "renderer/renderer.hpp"
#include "gui/raspGui.hpp"
#include "core/log.hpp"
#include "game.hpp"

#include "raylib.h"
#include "raymath.h"

#include <format>
#include <unordered_map>

static std::unordered_map<int, float> zooms;
static int zoomdex = 5;

Editor::Editor(Camera2D& camera) : m_Camera(camera) {
	zooms[1] = 5.0f;
	zooms[2] = 3.0f;
	zooms[3] = 2.0f;
	zooms[4] = 1.5f;
	zooms[5] = 1.0f;
	zooms[6] = 0.5f;
	zooms[7] = 0.3f;
	zooms[8] = 0.1f;
	zooms[9] = 0.05f;
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
	}

	// Panning (dragging doesn't affect anything)
	if (m_Mode == EditorModes::Pan) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			m_Camera.target = Vector2Add(m_Camera.target, Vector2Scale(GetMouseDelta(), Game.deltaTime * -900));
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

	if (m_Searching) {
		if (!IsKeyPressed(0)) {
			Log(LogLevel::Info, std::format("{}", (uint32_t)GetKeyPressed()));
			m_Search.append(1, GetCharPressed());
		}
	} else if (!m_Search.empty() ){
		m_CurrentTile = m_Search;
		Log(LogLevel::Info, m_Search);
	}
}

void Editor::PlaceTile() {
	bool canPlace = true;
	for (auto& tile : m_Tiles) {
		if (Vector2Equals(tile.GetPosition().RaylibVector(),
				 {std::floor(((GetMouseX() + m_Camera.target.x * m_Camera.zoom) / (m_Camera.zoom * 64.0f))), 
				  std::floor(((GetMouseY() + m_Camera.target.y * m_Camera.zoom) / (m_Camera.zoom * 64.0f))) })) 
		{
			Log(LogLevel::Info, "Cannot place block here!");
			canPlace = false;
		}
	}

	if (canPlace) {
		m_Tiles.push_back(Tile(m_CurrentTile, 
					  {static_cast<int32_t>( ((GetMouseX() + m_Camera.target.x * m_Camera.zoom) / (m_Camera.zoom * 64.0f)) ), 
					   static_cast<int32_t>( ((GetMouseY() + m_Camera.target.y * m_Camera.zoom) / (m_Camera.zoom * 64.0f)) )}));
		Log(LogLevel::Info, "place");
	}
}

void Editor::OnRender() {
	BeginMode2D(m_Camera);

	for (auto& tile : m_Tiles) {
		tile.OnRender();
	}

	EndMode2D();

	if (m_Searching) {
		RaspGui::Button({0.5f, 0.5f, 0.3f, 0.3f}, "hi");
	}
}
