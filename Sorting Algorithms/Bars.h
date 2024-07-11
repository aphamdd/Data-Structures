#pragma once
#include "SFML/Graphics.hpp"
#include "Entity.h"

class Bars : public Entity {
public:
  Bars(const sf::Vector2f pos, const sf::Vector2f size);

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void update() override;

private:
  sf::RectangleShape m_shape;
};
