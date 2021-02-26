#include "Boss.h"
#include <random>
#include <iostream>

/** Boss constructor implementation.
*/
Boss::Boss(int stage)
  : Character{(1000 + (10 * (stage * stage))), 1, "static/boss_sprite.png"}
{
  setPosition(800, 600);
  setScale(4, 4);
}

/** Boss shoot implementation.
 * Generates two random numbers, one for height and width, dis;
 * and one for the y-position, dis2, of the projectile.
 */
void Boss::shoot(std::vector<Object*>& objects)
{
  std::random_device rd{};
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(2, 6);
  std::uniform_int_distribution<> dis2(0, 300);

  objects.push_back(new Projectile{-2, static_cast<float>(dis(gen)*-0.05),
                        static_cast<float>(dis(gen)*-0.05), getPosition().x - 25,
                        getPosition().y + dis2(gen), 0.5});
}

/** Boss implementation of update.
 * The random generator generates a number for the shoot_clock to compare to,
 * introducing some randomness in the boss pattern.
 */
void Boss::update(std::vector<Object*>& objects,
                  [[maybe_unused]]Object* boss, Object* player)
{
  std::random_device rd{};
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(300, 1200);
  if (getGlobalBounds().top + getGlobalBounds().height
      >= 725.f)
  {
    fall_speed = -20;
    falling = false;
  }
  else if (getGlobalBounds().top < 275.f)
  {
    fall_speed = 1;
    falling = true;
  }

  if ( shoot_clock.getElapsedTime().asMilliseconds() >= dis(gen))
  {
    shoot(objects);
    shoot_clock.restart();
  }
}

/** Draws the boss and
 * boss-hp on the screen.
 */
void Boss::render(sf::RenderWindow& drawto)
{
  drawto.draw(*this);
  sf::Font font;
  font.loadFromFile("static/m3x6.ttf");

  sf::Text text{"Hp: " + std::to_string(health_points), font};
  text.setPosition(434.f, 5.f);
  drawto.draw(text);
}
