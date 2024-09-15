#pragma once
#include "LLNode.h"

LLNode::LLNode(const sf::Vector2f pos) : next(NULL), data(0) { 
  shape.setSize(sf::Vector2f(100.f, 50.f));
  shape.setPosition(pos);
  shape.setFillColor(sf::Color::White);
  shape.setOutlineThickness(1.5f);
  shape.setOutlineColor(sf::Color::Black);
  shape.setOrigin(sf::Vector2f(50.f, 25.f));
}

LLNode::~LLNode() {
  // TODO: handle proper memory management in destructor
}

void LLNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = NULL;
  target.draw(shape, states);
}
