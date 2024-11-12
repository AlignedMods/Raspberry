#pragma once

#include "raylib.h"

class Collectable {
public:
    Collectable();
    ~Collectable();

    virtual void InitTextures();

    virtual void OnTouch();
    virtual void OnUpdate();

    Vector2 GetTilePos() const;
    void Update();

protected:
    Vector2 m_Pos;

    Texture2D m_Texture;
};
