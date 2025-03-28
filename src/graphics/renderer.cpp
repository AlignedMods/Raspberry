#include "renderer.hpp"

#include "core/core.hpp"
#include "raylib.h"

void Renderer::RenderTexture(Texture& texture, float x, float y) {
  if (texture.width != texture.height) {
    Error("Textures must be square!", -2);
  } else if (texture.width != 16) {
    Error("Textures must be 16x16!", -3);
  } else {
    DrawTextureEx(texture, {x, y}, 0.0f, 4.0f, WHITE);
  }
}
