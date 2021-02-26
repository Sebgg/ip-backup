#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Object.h"
#include "Character.h"

class Projectile: public Object
{
public:
  Projectile(int const, float const, float const,
             float const, float const, double const);
  ~Projectile() = default;

  void render(sf::RenderWindow&) override;

  void update(std::vector<Object*>&, Object*, Object*) override;

  float get_height() const;

  float get_width() const;

  int invert_direction();

  int get_direction() const;

protected:
  int direction{}; /*!< Stores if the projectile goes left or right. */
  double const damage_modifier{}; /*!< Stores how much damage the projectile does. */

  float const height{}; /*!< Stores the height of the projectile. */
  float const width{}; /*!< Stores the width of the projectile. */
};

#endif //PROJECTILE_H
