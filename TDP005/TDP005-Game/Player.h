#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Projectile.h"
#include "Platform.h"

class Player: public Character
{
public:
  Player();
  ~Player() = default;

  void shoot(std::vector<Object*>&) override;

  void update(std::vector<Object*>&, Object*, Object*) override;

  void render(sf::RenderWindow&) override;

  bool check_parry(Object*, Projectile*) const;

private:
  sf::Clock invul_clock{}; /*!< Clock to keep time of invulnerability. */
  sf::Clock parry_clock{}; /*!< Clock to keep time of parrying cooldown. */
};

#endif //PLAYER_H
