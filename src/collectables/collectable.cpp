#include "collectable.hpp"
#include "game.hpp"

void Collectable::Draw() {
	Game::GetRenderer()->RenderTexture(m_Texture, m_Pos.x * 64, m_Pos.y * 64);
}

TilePos Collectable::GetTilePos() const {
	return m_Pos;
}
