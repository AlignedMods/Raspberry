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

	float Approach(float end, float current, float interval);

    void UpdateTextures();

private:
    void CheckCollisionsH();
    void CheckCollisionsV();

    const float m_Gravity = 30.0f;

	Vector2 m_WalkSpeed;
	Vector2 m_DashSpeed;

    bool m_Grounded;

	double m_JumpCooldown = 0.0;
	double m_TimeSinceDash = 0.0;

	double m_CoyoteTimeCounter = 0.0;

	bool m_Dashing = false;

    std::map<std::string, Texture> m_Textures = {};
};
