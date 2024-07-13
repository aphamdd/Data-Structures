#pragma once
#include "SFML/Graphics.hpp"
#include "Entity.h"

class Bars : public Entity {
public:
  // parameterized constructor
  Bars(const sf::Vector2f pos, const sf::Vector2f size);
  /* copy constructor
  Bars(const Bars& other) : m_shape(other.m_shape) {}
  // copy assignment operator
  Bars& operator=(const Bars& other) {
    if (this != &other)
      m_shape = other.m_shape;
    return *this;
  }
  // move constructor
  Bars(Bars&& other) noexcept : m_shape(std::move(other.m_shape)) {}
  // move assignment operator
  Bars& operator=(Bars&& other) noexcept {
    if (this != &other)
      m_shape = std::move(other.m_shape);
    return *this;
  }
  */

public:
  sf::RectangleShape m_shape;

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
