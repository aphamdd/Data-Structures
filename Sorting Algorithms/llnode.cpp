#pragma once
#include "LLNode.h"

// TODO: properly handle text centering
LLNode::LLNode(const sf::Vector2f pos, sf::Text& text) :
  next(NULL),
  dataText(text),
  data(0), 
  ID(1) { 
  sf::Vector2f size(100.f, 50.f);
  shape.setSize(size);
  shape.setPosition(pos);
  shape.setFillColor(sf::Color::White);
  shape.setOutlineThickness(1.5f);
  shape.setOutlineColor(sf::Color::Black);
  shape.setOrigin(sf::Vector2f(shape.getSize().x/2, shape.getSize().y/2));
  sf::Vector2f textPos = pos;
  textPos.y += -size.y / 1.5;
  dataText.setPosition(textPos);
  dataText.setCharacterSize(50);
  dataText.setFillColor(sf::Color::Black);
  dataText.setString(std::to_string(ID));
}

LLNode::~LLNode() {
  next = NULL;
  data = 0;
  ID = 0;
  // TODO: delete shape??
}

void LLNode::update(const sf::Vector2f mpos) {
  shape.setPosition(mpos);

  sf::Vector2f textPos = shape.getPosition();
  textPos.y += -shape.getSize().y / 1.5;
  dataText.setPosition(textPos);
}

void LLNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = NULL;
  target.draw(shape, states);
  target.draw(dataText, states);
}
