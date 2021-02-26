#ifndef CHARACTER_H
#define CHARACTER_H

#include "Object.h"
#include <vector>

class Character: public Object
{
public:
  Character(double, int, std::string);
  ~Character();

  bool is_alive() const;

  virtual void shoot(std::vector<Object*>&) = 0;

  void take_damage(double const);

  void gravity(float const);

  void set_falling(bool const&);

  void set_fall_speed(int);

  int get_fall_speed() const;

  void set_on_platform(bool const);

protected:
  bool falling{}; /*!< Stores if character is falling or not. */
  bool on_platform{}; /*!< Stores if character is on platform. */

  double health_points{}; /*!< Stores health_points. */
  int damage_modifier{}; /*!< Stores damage output of projectiles. */
  int fall_speed{}; /*!< Stores how fast character is falling. */

  sf::Clock shoot_clock{}; /*!< Clock for buffering shots. */

  double get_health_points() const;
  int get_damage_modifier() const;
};

#endif //CHARACTER_H
