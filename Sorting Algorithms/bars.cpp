#include "Bars.h"

Bars::Bars(const sf::Vector2f pos, const sf::Vector2f size) : Entity(pos, size) {
  m_shape.setPosition(m_pos);
  m_shape.setFillColor(sf::Color(sf::Color::Green));
  m_shape.setSize(m_size);
}

void Bars::update() {
  return;
}

void Bars::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = NULL;
  target.draw(m_shape, states);
}