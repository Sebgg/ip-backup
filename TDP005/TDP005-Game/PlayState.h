#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.hcc"
#include "Object.h"
#include "Character.h"
#include "Player.h"
#include "Boss.h"
#include "Platform.h"
#include "Projectile.h"

#include <vector>
#include <algorithm>

class PlayState
{
public:
  PlayState(int);
  ~PlayState();

  void update_screen();

  void update_gravity(float const);

  void draw_screen(sf::RenderWindow&);

  bool check_win() const;

  bool check_loss() const;

  void display_win(sf::RenderWindow&) const;

  void display_loss(sf::RenderWindow&) const;

private:
  std::vector<Object*> objects{}; /*!< Vector with all platforms and projectiles. */
  Character* player{}; /*!< Pointer to the player. */
  Character* boss{}; /*!< Pointer to the boss. */
  int stage{}; /*!< Current stage the player is on. */
};

#endif //PLAYSTATE_H
