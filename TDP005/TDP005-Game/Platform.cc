#include "Platform.h"
#include <iostream>

/** Platform constructor implementation.
*/
Platform::Platform(float x = 0, float y = 0,
                   double width = 0, double height = 0)
  : Object{"static/platform.png"}, has_player{false}
{
  setPosition(x, y);
  setScale(( width / 25 ), ( height / 25 ));
}

/** Draws the platform on the screen.
*/
void Platform::render(sf::RenderWindow& drawto)
{
  drawto.draw(*this);
}

/** update is run every frame and checks whether or not there is a player
 *  or boss standing on it.
 */
void Platform::update([[maybe_unused]]std::vector<Object*>& objects,
                      Object* player, Object* boss)
{
  if (Character* p{dynamic_cast<Character*>(player)})
  {
    float p_bottom{p->getGlobalBounds().top - 20 + p->getGlobalBounds().height};
    float p_right{p->getGlobalBounds().left + p->getGlobalBounds().width - 40};

    if (p_bottom >= getGlobalBounds().top - 5 &&
        p_bottom <= getGlobalBounds().top + 5 &&
        p_right > getGlobalBounds().left - 20 &&
        p_right < (getGlobalBounds().left + getGlobalBounds().width) + 20 &&
        p->get_fall_speed() > 0 && !has_player)
    {
      p->set_falling(false);
      p->set_fall_speed(0);
      p->set_on_platform(true);
      has_player = true;
    }
    else if (has_player)
    {
      p->set_falling(true);
      p->set_on_platform(false);
      has_player = false;
    }
  }
}
