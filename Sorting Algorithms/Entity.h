#pragma once
#include "common.h"
#include "SFML/Graphics.hpp"

class Entity : public sf::Drawable, public sf::Transformable {
public:
  Entity(const sf::Vector2f pos, const sf::Vector2f size) :
    m_pos(pos),
    m_size(size)
  {}

  virtual void update() = 0;
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
  float dt() { return dtClock.restart().asSeconds(); }

  sf::Clock dtClock;
  sf::Vector2f m_pos;
  sf::Vector2f m_size;
};