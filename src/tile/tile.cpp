#include "tile.hpp"
#include "core/log.hpp"
#include "renderer/renderer.hpp"
#include "core/types.hpp"
#include "registry.hpp"

Tile::Tile(const std::string& type, const TilePosition& position) {
	m_Type = type;
	m_Position = position;

	m_Texture = Registry.GetTileTexture(type);
}

void Tile::OnRender() {
	OnRender(0xffffffff);
}

void Tile::OnRender(uint32_t flag) {
	Renderer.RenderTexture(m_Texture, m_Position.RaylibVector().x, m_Position.RaylibVector().y, GetColor(flag));
}

TilePosition Tile::GetPosition() {
	return m_Position;
}

std::string& Tile::GetType() {
	return m_Type;
}
