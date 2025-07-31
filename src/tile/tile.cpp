#include "tile.hpp"
#include "renderer/renderer.hpp"
#include "core/types.hpp"
#include "registry.hpp"

#include "raylib.h"

f32 ffs = 1.0f;

Tile::Tile(const std::string& type, const i2& position) {
	m_Type = type;
	m_Position = position;
    m_RealPosition = {position.x * 64.0f, position.y * 64.0f};

	m_Texture = Registry.GetTileTexture(type);

    brightness = ffs;
    ffs -= 0.05f;
}

void Tile::OnRender() {
    Color col = {(u8)(brightness * 255.0f), (u8)(brightness * 255.0f), (u8)(brightness * 255.0f), 255};
	OnRender(ColorToInt(col));
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
