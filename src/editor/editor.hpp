#pragma once

#include "core/types.hpp"
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

    i2 GetWorldPosition();
    void HandleSelection();
    void Export();

private:
    std::map<i2, Tile> m_Tiles;
    std::vector<Tile> m_Selection;

    std::string m_CurrentTile = "dirt";
    EditorModes m_Mode = EditorModes::Build;
    EditorStates m_State = EditorStates::Click;

    bool m_Searching = false;
    RaspGui::Input m_Input;

    f32 m_SelectionBrightness = 0.0f;
    f32 m_SelectionBrightnes = 0.0f;
    Vector3 hsv;
    //Color col = {0, 0, 0, 255};

    Camera2D& m_Camera;
};
