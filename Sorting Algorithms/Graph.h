#pragma once
#include "common.h"
#include "SFML/Graphics.hpp"
#include <random>

class Graph : public sf::Drawable, public sf::Transformable {
public:
  Graph(const int size);
  void shuffle();

public:
  std::vector<sf::RectangleShape> m_histogram;

private:
  void build(const int size);
  sf::RectangleShape initShape(const sf::Vector2f pos, const sf::Vector2f size);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};