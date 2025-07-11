#include "tile.hpp"
#include "blueberry/renderer/renderer.hpp"
#include "shared/log.hpp"
#include "shared/types.hpp"

#include <map>
#include <string>

static std::map<std::string, Texture> textures;
static bool loaded = false;

Tile::Tile(const std::string& type, const TilePosition& position) {
	m_Type = type;
	m_Position = position;

	if (!loaded) {
		Debug("pee");
		textures["Stone"] = LoadTexture("Assets/stone.png");
        textures["Dirt"] = LoadTexture("Assets/dirt.png");
        textures["Grass"] = LoadTexture("Assets/grass.png");
        textures["Brick"] = LoadTexture("Assets/brick.png");
        textures["Grasspatch"] = LoadTexture("Assets/grass_patch.png");

		loaded = true;
	}

	m_Texture = textures.at(type);
}

void Tile::OnRender() {
	Renderer.RenderTexture(m_Texture, m_Position.RaylibVector().x * 64.0f, m_Position.RaylibVector().y * 64.0f);
}

TilePosition Tile::GetPosition() {
	return m_Position;
}

std::string& Tile::GetType() {
	return m_Type;
}
