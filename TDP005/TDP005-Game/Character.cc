#include "Character.h"
#include <iostream>

/** Character constructor implementation.
*/
Character::Character(double health_points = 0, int damage_modifier = 0,
                     std::string texture_filename = "static/default.png")
 : Object{texture_filename},
   health_points{health_points},
   damage_modifier{damage_modifier},
   fall_speed{10},
   falling{true},
   on_platform{false}
{
}

/** Character destructor implementation.
*/
Character::~Character()
{}

bool Character::is_alive() const
{
  return health_points > 0;
}

/** Used to remove health_points from a character.
*/
void Character::take_damage(double const damage)
{
  health_points -= damage;
}

/** Checks if character is not standing on the ground or a platform.
 *  If it's not standing on the ground or platform we want to move it down
 *  to simulate gravity.
 */
void Character::gravity(float const height)
{
  if (((getGlobalBounds().top + getGlobalBounds().height
        < height) || fall_speed < 0) && falling)
  {
    move(0, 1*fall_speed);
    ++fall_speed;
  }
  else if (!falling && fall_speed < 0)
  {
    move(0, -5);
  }
}

double Character::get_health_points() const
{
  return health_points;
}

int Character::get_damage_modifier() const
{
  return damage_modifier;
}

void Character::set_falling(bool const& is_falling)
{
  falling = is_falling;
}

void Character::set_fall_speed(int new_fall_speed)
{
  fall_speed = new_fall_speed;
}

int Character::get_fall_speed() const
{
  return fall_speed;
}

void Character::set_on_platform(bool const on_plat)
{
  on_platform = on_plat;
}
