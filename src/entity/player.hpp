#pragma once

#include "entity/entity.hpp"

#include "raylib.h"

#include <map>
#include <string>

class Player : public Entity {
public:
    Player();

	void InitTextures();

    Player operator=(const Player& player);

    virtual void OnRender() override;
    virtual void OnUpdate() override;

    void UpdateTextures();

private:
    void CheckCollisions();

    float m_Acceleration;

    const float m_Gravity = 0.02f;

    bool m_OnGround;

    std::map<std::string, Texture> m_Textures = {};
};
