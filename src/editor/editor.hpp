#pragma once

#include "tile/tile.hpp"
#include "gui/raspGui.hpp"

#include "raylib.h"

#include <map>
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

    void OnUpdate();
    void OnRender();

private:
    void PlaceTile();
    void DestroyTile();

    TilePosition GetWorldPosition();
    void HandleSelection();
    void Export();

private:
    std::map<TilePosition, Tile> m_Tiles;
    std::vector<Tile> m_Selection;

    std::string m_CurrentTile = "dirt";
    EditorModes m_Mode = EditorModes::Build;
    EditorStates m_State = EditorStates::Click;

    bool m_Searching = false;
    RaspGui::Input m_Input;

    Camera2D& m_Camera;
};
