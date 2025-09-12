#include "entity/entity.hpp"
#include "game.hpp"
#include "log.hpp"
#include "types.hpp"

Entity Entity::operator=(const Entity& other) {
    return other;
}

float Entity::GetX() {
    return m_Position.x;
}

float Entity::GetY() {
    return m_Position.y;
}

Vector2& Entity::GetPos() {
    return m_Position;
}

void Entity::OffsetPositionByVelocity() {
    // m_Position.x += m_Velocity.x * Game.deltaTime;
    CheckCollisionsH();

    // m_Position.y += m_Velocity.y * Game.deltaTime;
    CheckCollisionsV();
}

void Entity::CheckCollisionsH() {
    // Level* level = Game.GetCurrentLevel();
    // auto& tiles = level->GetAllTiles();

    // for (auto& tile : tiles) {
    // 	if (CheckCollisionRecs({m_Position.x, m_Position.y + 0.1f, 1.0f, 0.9f},
    // 						   {tile.GetPosition().RaylibVector().x, tile.GetPosition().RaylibVector().y, 1.0f, 1.0f}))
    // 	{
    // 		if (m_Velocity.x > 0) {
    // 			m_Position.x = tile.GetPosition().RaylibVector().x - 1.0f;
    // 		} else if (m_Velocity.x < 0) {
    // 			m_Position.x = tile.GetPosition().RaylibVector().x + 1.0f;
    // 		}

    // 		m_Velocity.x = 0;
    // 	}
    // }
}

void Entity::CheckCollisionsV() {
    // Level* level = Game.GetCurrentLevel();

    m_Grounded = false;

    // for (auto& tile : level->GetAllTiles()) {
    // 	if (CheckCollisionRecs({m_Position.x, m_Position.y + 0.1f, 1.0f, 0.9f},
    // 						   {tile.GetPosition().RaylibVector().x, tile.GetPosition().RaylibVector().y, 1.0f, 1.0f}))
    // 	{
    // 		if (m_Velocity.y > 0) {
    // 			m_Position.y = tile.GetPosition().RaylibVector().y - 1.0f;
    // 			m_Grounded = true;
    // 		} else if (m_Velocity.y < 0) {
    // 			m_Position.y = tile.GetPosition().RaylibVector().y + 1.0f;
    // 		}

    // 		m_Velocity.y = 0;
    // 	}
    // }
}

float Entity::Approach(float end, float current, float interval) {
    float val = current;

    if (end > current) {
        val = std::min(current + interval, end);
    } else {
        val = std::max(current - interval, end);
    }

    return val;
}

void Entity::UpdateTextures() {
    m_Anim++;

    // m_Texture = m_Textures.at("IdleLeft");

    if (m_Direction == 0) {
        // m_Texture = m_Textures.at("IdleLeft");
        m_Flip = false;

        if (m_Walking && m_Velocity.x < 0.0f) {
            if (m_Anim % 10 > 5) {
                // m_Texture = m_Textures.at("RunningLeftUp");
            } else {
                // m_Texture = m_Textures.at("RunningLeftDown");
            }
        }
    }

    if (m_Direction == 1) {
        // m_Texture = m_Textures.at("IdleRight");
        m_Flip = true;

        if (m_Walking && m_Velocity.x > 0.0f) {
            if (m_Anim % 10 > 5) {
                // m_Texture = m_Textures.at("RunningRightUp");
            } else {
                // m_Texture = m_Textures.at("RunningRightDown");
            }
        }
    }
}
