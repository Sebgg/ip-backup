#include "Player.h"
#include <iostream>

/** Player constructor implementation.
*/
Player::Player()
 : Character{10, 1, "static/player_sprite.png"}
{
  setPosition(100, 300);
  setScale(2, 2);
}

/** update is run every frame and checks for keypresses that control moving,
 *  shooting and parrying. It also checks for collision between player and boss.
 */
void Player::update(std::vector<Object*>& objects, Object* player, Object* boss)
{
  if (player->getGlobalBounds().intersects(boss->getGlobalBounds()) &&
      invul_clock.getElapsedTime().asSeconds() > 3)
  {
    take_damage(5);
    invul_clock.restart();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
  {
    if (getGlobalBounds().top + getGlobalBounds().height
        >= 725.f || on_platform)
    {
      fall_speed = -20;
      falling = true;
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
  {
    if (getPosition().x + getGlobalBounds().width < 928)
    {
      move(5, 0);
    }
    else if (getPosition().x == 928)
    {
      move(-1, 0);
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
  {
    if (on_platform)
    {
      move(0, 10);
      falling = true;
      fall_speed = 1;
      on_platform = false;
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
  {
    if (getPosition().x > 0)
    {
      move(-5, 0);
    }
    else if (getPosition().x == 0)
    {
      move(1, 0);
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
  {
    if (shoot_clock.getElapsedTime().asMilliseconds() >= 300)
    {
      shoot(objects);
      shoot_clock.restart();
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
  {
    for (Object* object : objects)
    {
      if (Projectile* proj{dynamic_cast<Projectile*>(object)})
      {
        if (check_parry(player, proj))
        {
          proj->invert_direction();
          parry_clock.restart();
        }
      }
    }
  }
}

/** Creates a new object of type Projectile in front of players position.
 *  We then insert this object into a vector of objects.
 */
void Player::shoot(std::vector<Object*>& objects)
{
  Object* p = new Projectile{1, 0.1, 0.1,
                      getPosition().x + 100.f, getPosition().y, 1.5};
  p->setColor(sf::Color::Magenta);
  objects.insert(objects.end(), p);
}

/** Checks if a projectile is within a certain distance (200.f here) in fron of
 *  the player. If it is inside this area and the parry timer for player is
 *  higher than 3 the function returns true.
 */
bool Player::check_parry(Object* player, Projectile* proj) const
{
  return (player->getPosition().x + 200.f >= proj->getPosition().x &&
          player->getPosition().x <= proj->getPosition().x &&
          player->getGlobalBounds().top <= proj->getPosition().y &&
          player->getGlobalBounds().top + player->getGlobalBounds().height >=
          proj->getPosition().y &&
          parry_clock.getElapsedTime().asSeconds() > 3 &&
          proj->get_direction() < 0);
}

/** Draws the player and
 * player-hp on the screen.
 */
void Player::render(sf::RenderWindow& drawto)
{
  drawto.draw(*this);
  sf::Font font;
  font.loadFromFile("static/m3x6.ttf");

  sf::Text text{"Hp: " + std::to_string(health_points), font};
  text.setPosition(5.f, 5.f);
  drawto.draw(text);
}
