#include "tile.hpp"
#include "core/log.hpp"
#include "renderer/renderer.hpp"
#include "core/types.hpp"

#include <unordered_map>

static std::unordered_map<std::string, Texture> textures;
static bool loaded = false;

Tile::Tile(const std::string& type, const TilePosition& position) {
	m_Type = type;
	m_Position = position;

	if (!loaded) {
		textures["Stone"] = LoadTexture("Assets/stone.png");
        textures["Dirt"] = LoadTexture("Assets/dirt.png");
        textures["Grass"] = LoadTexture("Assets/grass.png");
        textures["Brick"] = LoadTexture("Assets/brick.png");
        textures["Grasspatch"] = LoadTexture("Assets/grass_patch.png");

		loaded = true;
	}

	m_Texture = textures.at(m_Type);
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
