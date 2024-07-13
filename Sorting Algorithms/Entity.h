#pragma once
#include "common.h"
#include "SFML/Graphics.hpp"

class Entity : public sf::Drawable, public sf::Transformable {
public:
  Entity() {
    m_pos = sf::Vector2f(300.f, 300.f);
    m_size = sf::Vector2f(10.f, 10.f);
  }
  Entity(const sf::Vector2f pos, const sf::Vector2f size) :
    m_pos(pos),
    m_size(size)
  {}

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

public:
  sf::Vector2f m_pos;
  sf::Vector2f m_size;
};