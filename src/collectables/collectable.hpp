#pragma once

#include "raylib.h"

class Collectable {
public:
    Collectable();
    ~Collectable();

    virtual void InitTextures();

    virtual void OnTouch();
    virtual void OnUpdate();

    Vector2& GetTilePos();
    void Update();

    void OnRender();

protected:
    Vector2 m_Pos;

    Texture m_Texture;
};
