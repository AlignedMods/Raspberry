#pragma once

#include "raspberry/entity/entity.hpp"
#include "raspberry/level/level.hpp"

#include "raylib.h"

#include <map>
#include <string>
#include <memory>

class Level;

class Player : public Entity {
public:
    Player();

    void operator=(const Player& player);

    virtual void OnRender() override;
    virtual void OnUpdate() override;

    void UpdateTextures();

private:
    void CheckCollisions();

    float m_Acceleration;

    const float m_Gravity = 0.02f;

    bool m_OnGround;

    std::shared_ptr<Level> m_Level = nullptr;

    std::map<std::string, Texture> m_Textures = {};
};
