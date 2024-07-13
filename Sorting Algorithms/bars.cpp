#include "Bars.h"
#include <iostream>
using namespace std;

Bars::Bars(const sf::Vector2f pos, const sf::Vector2f size) : 
  Entity(pos, size) {
  m_shape.setPosition(m_pos);
  m_shape.setFillColor(sf::Color::White);
  m_shape.setOutlineThickness(1.f);
  m_shape.setOutlineColor(sf::Color::Black);
  m_shape.setSize(m_size);
  m_shape.setOrigin(sf::Vector2f(0, m_size.y));
}
 
Bars Bars::test(Bars& bar) {
  Bars temp = bar;
  temp.m_shape.setFillColor(sf::Color::Yellow);
  temp.m_shape.setPosition(sf::Vector2f(400.f, 400.f));

  return temp;
}

void Bars::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = NULL;
  target.draw(m_shape, states);
}