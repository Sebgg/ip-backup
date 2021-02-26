#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <string>

class Object: public sf::Sprite
{
public:
  Object(std::string);
  virtual ~Object() = default;

  virtual void render(sf::RenderWindow&) = 0;
  virtual void update(std::vector<Object*>&, Object*, Object*) = 0;

protected:
  /* Variable declarations */
  sf::Texture texture{}; /*!< Stores the texture for the object. */
};

#endif //OBJECT_H
