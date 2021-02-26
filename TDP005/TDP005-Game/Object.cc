#include "Object.h"
#include "Character.h"

/** Object constructor implementation.
*/
Object::Object(std::string sprite_name)
 :texture{sf::Texture{}}
{
  texture.loadFromFile(sprite_name);
  setTexture(texture);
}
