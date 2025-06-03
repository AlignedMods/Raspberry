#include "game.hpp"

int main() {
  // game is a singleton which holds all of the information of the current game
  Game game;
  game.Run();

  return 0;
}