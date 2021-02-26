#include "Projectile.h"

/** Projectile constructor implementation.
*/
Projectile::Projectile(int const direction, float const width,
                       float const height, float const x, float const y,
                       double const damage_modifier)
 : Object{"static/projectile.png"}, direction{direction},
   damage_modifier{damage_modifier}
{
  setPosition(x, y);
  setScale(width, height);
}

/** Projectile's implementation of update.
 * Despawns this if it hits the player or boss, or goes out of bounds.
 */
void Projectile::update(std::vector<Object*>& objects,
                        Object* player, Object* boss)
{
  Character* _player{dynamic_cast<Character*>(player)};
  Character* _boss{dynamic_cast<Character*>(boss)};
  if (getPosition().x > 928 || getPosition().x < 0)
  {
    delete this;
    objects.erase(std::find(objects.begin(), objects.end(), this));
  }
  else if (getGlobalBounds().intersects(_player->getGlobalBounds()))
  {
    delete this;
    _player->take_damage(1 * damage_modifier);
    objects.erase(std::find(objects.begin(), objects.end(), this));
  }
  else if (getGlobalBounds().intersects(_boss->getGlobalBounds()))
  {
    delete this;
    _boss->take_damage(10 * damage_modifier);
    objects.erase(std::find(objects.begin(), objects.end(), this));
  }
  else
  {
    move(5*direction, 0);
  }
}

/** Draws the projectile on the screen.
 */
void Projectile::render(sf::RenderWindow& drawto)
{
  drawto.draw(*this);
}

float Projectile::get_height() const
{
  return height;
}

float Projectile::get_width() const
{
  return width;
}

/** Inverts the direction the projectile is going in, and flips the sprite.
 */
int Projectile::invert_direction()
{
  direction *= -1;
  setScale(getScale().x * -1, getScale().y);
}

int Projectile::get_direction() const
{
  return direction;
}
