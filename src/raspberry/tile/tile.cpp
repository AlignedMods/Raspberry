#include "tile.hpp"
#include <memory>
#include "raspberry/renderer/renderer.hpp"
#include "shared/types.hpp"

Tile::Tile(const std::string& type, const TilePosition& position) {
	m_Type = type;
	m_Position = position;
}

void Tile::OnRender() {
	Renderer.RenderTexture(m_Texture, m_Position.RaylibVector().x, m_Position.RaylibVector().y);
}

TilePosition Tile::GetPosition() {
	return m_Position;
}

std::string& Tile::GetType() {
	return m_Type;
}
