#pragma once

#include "raylib.h"

class Renderer {
 public:
  void RenderTexture(Texture& texture, float x, float y);
  void RenderText(const char* txt, int x, int y);
};
