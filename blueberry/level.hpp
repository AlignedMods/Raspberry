#pragma once

#include "types.hpp"
#include "editortile.hpp"

#include <vector>

class Level {
public:
    void OnUpdate();
    void OnRender();
    void Tick();

    void AddTile(EditorTile tile);
    std::vector<EditorTile>& GetAllTiles();

    void SetFound();

    bool IsCollectableFound();

    void SetName(const std::string& name);
    std::string& GetName();

private:
    std::vector<EditorTile> m_Tiles;

    std::string m_Name;

    f32 m_Brightness = 1.0f;
    Shader m_Shader;

    bool m_CollectableFound;
};
