#include "Bars.h"

Bars::Bars(const sf::Vector2f pos, const sf::Vector2f size) : 
  Entity(pos, size) {
  m_shape.setPosition(m_pos);
  m_shape.setFillColor(sf::Color::Black);
  m_shape.setOutlineThickness(1.f);
  m_shape.setOutlineColor(sf::Color::White);
  m_shape.setSize(m_size);
  m_shape.setOrigin(sf::Vector2f(0, m_size.y));
}

void Bars::update() {
  return;
}

void Bars::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = NULL;
  target.draw(m_shape, states);
}