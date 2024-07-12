#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Container : public sf::Drawable, public sf::Transformable {
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};