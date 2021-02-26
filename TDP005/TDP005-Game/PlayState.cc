#include "PlayState.h"
#include <iostream>

/** PlayState constructor implementation.
*/
PlayState::PlayState(int stage = 1)
  : stage{stage}, boss{new Boss{stage}}, player{new Player{}}
{
  objects.push_back(new Platform(175, 550, 10, 5));
  objects.push_back(new Platform(550, 550, 10, 5));
  objects.push_back(new Platform(362.5, 400, 10, 7));
}

/** Playstate destructor implementation.
 */
PlayState::~PlayState()
{
  for (Object* object : objects)
  {
    delete object;
  }
  objects.clear();
  delete player;
  delete boss;
}

/** PlayState implementation of update_screen.
 * Calls the update function on all onscreen Objects.
 */
void PlayState::update_screen()
{
  player->update(objects, player, boss);
  std::vector<Object*> copy{objects};
  for (Object* object : objects)
  {
    object->update(copy, player, boss);
  }
  boss->update(copy, player, boss);
  objects.swap(copy);
}

/** PlayState implementation of update_gravity.
 * Calls the gravity function for player and boss.
 */
void PlayState::update_gravity(float const height)
{
  player->gravity(height);
  boss->gravity(height);
}

/** PlayState implementation of draw_screen.
 * Draws everything that is shown on the screen.
 */
void PlayState::draw_screen(sf::RenderWindow& drawto)
{
  //Creates background
  sf::Texture bg_texture;
  bg_texture.loadFromFile("static/Background.png");
  sf::Sprite background(bg_texture);
  drawto.draw(background);

  for (Object* object : objects)
  {
    object->render(drawto);
  }
  boss->render(drawto);
  player->render(drawto);
  sf::Texture fg_texture;
  fg_texture.loadFromFile("static/foreground.png");
  sf::Sprite foreground(fg_texture);
  drawto.draw(foreground);
}

bool PlayState::check_win() const
{
return player->is_alive() && !boss->is_alive();
}

bool PlayState::check_loss() const
{
  return !player->is_alive() && boss->is_alive();
}

/** PlayState implementation of display_win.
 * Draws the win screen if the player won.
 */
void PlayState::display_win(sf::RenderWindow& drawto) const
{
  drawto.clear(sf::Color::Black);
  sf::Font font{};
  font.loadFromFile("static/m3x6.ttf");
  sf::Text text{"YOU WON!", font, 256};
  text.setColor(sf::Color::Green);
  text.setPosition(200, 200);
  drawto.draw(text);
  drawto.display();
}

/** PlayState implementation of display_loss.
 * Draws the lose screen if the player lost.
 */
void PlayState::display_loss(sf::RenderWindow& drawto) const
{
  drawto.clear(sf::Color::Black);
  sf::Font font{};
  font.loadFromFile("static/m3x6.ttf");
  sf::Text text{"YOU LOST!", font, 256};
  text.setColor(sf::Color::Red);
  text.setPosition(200, 200);
  drawto.draw(text);
  drawto.display();
}
