#include "tile.hpp"
#include "renderer/renderer.hpp"
#include "types.hpp"
#include "registry/registry.hpp"

#include "raylib.h"

Tile::Tile(const std::string& type, const i2& position) {
	m_Type = type;
	m_Position = position;
    m_RealPosition = {position.x * 32.0f, position.y * 32.0f};

	m_Texture = Registry.GetTexture(type);
}

void Tile::OnRender() {
	OnRender(tint);
}

void Tile::OnRender(u32 flag) {
	Renderer.RenderTexture(m_Texture, m_Position.RaylibVector().x, m_Position.RaylibVector().y, GetColor(flag));
}

i2& Tile::GetPosition() {
	return m_Position;
}

f2& Tile::GetRealPosition() {
    return m_RealPosition;
}

std::string& Tile::GetType() {
	return m_Type;
}
