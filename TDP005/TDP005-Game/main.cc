#include "GameEngine.h"
#include <cstdlib>


int main() {
  setenv("GLIBCXX_FORCE_NEW", "1", true);
  GameEngine* game{new GameEngine{}};
  game->run();
  delete game;
  return 0;
}
