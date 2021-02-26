#ifndef PLATFORM_H
#define PLATFORM_H

#include "Object.h"
#include "Character.h"

class Platform: public Object
{
public:
  Platform(float, float, double, double);
  ~Platform() = default;

  void render(sf::RenderWindow&) override;
  void update(std::vector<Object*>&, Object*, Object*) override;
private:
  bool has_player{}; /*!< Stores whether or not the platform has a character on it */
};

#endif //PLATFORM_H
