#pragma once

#include "tile/tile.hpp"
#include "types.hpp"

#include "raylib.h"

#include <map>

class Entity {
public:
	virtual ~Entity() {}

    Entity operator=(const Entity& player);

	virtual void InitTextures() {}

	virtual void OnUpdate() {}
	virtual void OnRender() {}

	float GetX();
	float GetY();

	f2& GetPos();

    void UpdateTextures();

protected:
	void OffsetPositionByVelocity();

    void CheckCollisionsH();
    void CheckCollisionsV();

	float Approach(float end, float current, float interval);

protected:
	f2 m_Velocity;
	f2 m_Position = { 0.0f, 0.0f };

	// vectors for individual speeds
	f2 m_WalkSpeed;
	f2 m_DashSpeed;

	bool m_Grounded;
	bool m_Walking;

    const float m_Gravity = 30.0f;

	u32 m_Direction;

	Texture m_Texture;
    bool m_Flip = false;

	f64 m_JumpCooldown = 0.0;
	f64 m_TimeSinceDash = 0.0;

	f64 m_CoyoteTimeCounter = 0.0;

    std::map<std::string, Texture> m_Textures = {};

	u32 m_Anim = 0;
};
