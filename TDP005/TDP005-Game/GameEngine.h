#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Boss.h"
#include "Player.h"
#include "PlayState.h"
#include "GameState.hcc"

struct GameEngine
{
  GameEngine();
  ~GameEngine() = default;

  int run();

  PlayState* current_state{};
};

#endif //GAMEENGINE_H
