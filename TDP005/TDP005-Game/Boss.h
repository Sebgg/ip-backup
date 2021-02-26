#ifndef BOSS_H
#define BOSS_H

#include "Character.h"
#include "Projectile.h"
#include <vector>

class Boss: public Character
{
public:
  Boss(int);
  ~Boss() = default;

  void shoot(std::vector<Object*>&) override;

  void update(std::vector<Object*>&, Object*, Object*) override;

  void render(sf::RenderWindow&) override;
};

#endif //BOSS_H
