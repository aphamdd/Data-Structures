#pragma once
#include "LLNode.h"

// TODO: properly handle text centering
LLNode::LLNode(const sf::Vector2f pos, sf::Text& text) :
  next(NULL),
  dataText(text),
  nextLine(sf::Lines, 2),
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
  textPos.x -= size.x / 2;
  textPos.y -= size.y / 1.5;
  dataText.setPosition(textPos);
  dataText.setCharacterSize(50);
  dataText.setFillColor(sf::Color::Black);
  dataText.setString(std::to_string(ID));

  sf::Vector2f nextPos = pos;
  nextPos.x += size.x / 2;
  nextLine[0].position = nextPos;
  nextLine[1].position = nextPos;
  nextLine[0].color = sf::Color::Yellow;
  nextLine[1].color = sf::Color::Yellow;
}

LLNode::~LLNode() {
  next = NULL;
  data = 0;
  ID = 0;
  // TODO: delete shape??
}

void LLNode::update(const sf::Vector2f pos, LLNode* prev) {
  // TODO: handle boundaries and collision
  sf::Vector2f textPos = pos;
  textPos.x -= shape.getSize().x / 2;
  textPos.y -= shape.getSize().y / 1.5;
  dataText.setPosition(textPos);
  shape.setPosition(pos);

  // I need a previous pointer to update the next pointer line accordingly
  updateNext(prev);
}

// overloaded method: updates the previous node's line as well
void LLNode::updateNext(LLNode* prev) {
  // two cases:
  // am I the tail node
  // am I not a tail node

  // update current node next line
  sf::Vector2f nextPos = shape.getPosition();
  nextPos.x += shape.getSize().x / 2;
  nextLine[0].position = nextPos;
  // if theres no node in front, keep it hidden
  if (!next) {
    nextLine[1].position = nextPos;
  }

  // update the next line behind the current node
  sf::Vector2f prevPos = shape.getPosition();
  prevPos.x -= shape.getSize().x / 2;
  if (prev) {
    prev->nextLine[1].position = prevPos;
  }
}

void LLNode::updateNext() {
  // update current node next line
  sf::Vector2f nextPos = shape.getPosition();
  nextPos.x += shape.getSize().x / 2;
  nextLine[0].position = nextPos;
  // if theres no node in front, keep it hidden
  if (!next) {
    nextLine[1].position = nextPos;
  }
}

void LLNode::updateText(const int val) {
  if (val < 0 || val >= 100)
    return;

  std::cout << "update text" << std::endl;
  ID = val;
  std::cout << ID << std::endl;
  dataText.setString(std::to_string(ID));
}

void LLNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = NULL;
  target.draw(shape, states);
  target.draw(dataText, states);
  target.draw(nextLine, states);
}
