#pragma once
#include "LLNode.h"

// TODO: properly handle text centering
LLNode::LLNode(const sf::Vector2f pos, sf::Text& text, sf::Texture& texture) :
  dataText(text),
  nextLine(sf::Lines, 2),
  size(150.f, 75.f),
  shiftFactor(2.1, 1.8) {
  sprite.setTexture(texture);
  sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
  sprite.setOrigin(size.x * 0.5, size.y * 0.5);
  sprite.setPosition(pos);
  sprite.setColor(sf::Color::White);

  sf::Vector2f textPos = pos;
  textPos.x -= size.x / shiftFactor.x;
  textPos.y -= size.y / shiftFactor.y;
  dataText.setPosition(textPos);
  dataText.setCharacterSize(size.y/1.2);
  dataText.setFillColor(sf::Color::Black);
  dataText.setString(std::to_string(ID));

  sf::Vector2f nextPos = pos;
  nextPos.x += size.x * 0.5;
  nextLine[0].position = nextPos;
  nextLine[1].position = nextPos;
  nextLine[0].color = sf::Color::Yellow;
  nextLine[1].color = sf::Color::Yellow;
}

// TODO: delete sprite?? I don't create anything on the heap here
LLNode::~LLNode() {
  next = nullptr;
  ID = 0;
}

void LLNode::update(const sf::Vector2f pos, LLNode* prev) {
  sf::Vector2f textPos = pos;
  textPos.x -= size.x / shiftFactor.x;
  textPos.y -= size.y / shiftFactor.y;
  dataText.setPosition(textPos);
  sprite.setPosition(pos);

  // I need a previous pointer to update the next pointer line accordingly
  updateNext(prev);
}

// overloaded method: updates the previous node's line as well
void LLNode::updateNext(LLNode* prev) {
  // two cases:
  // am I the tail node
  // am I not a tail node

  // update current node next line
  sf::Vector2f nextPos = sprite.getPosition();
  nextPos.x += size.x / 2;
  nextLine[0].position = nextPos;
  // if theres no node in front, keep it hidden
  if (!next) {
    nextLine[1].position = nextPos;
  }

  // update the next line behind the current node
  sf::Vector2f prevPos = sprite.getPosition();
  prevPos.x -= size.x / 2;
  if (prev) {
    prev->nextLine[1].position = prevPos;
  }
}

void LLNode::updateNext() {
  // update current node next line
  sf::Vector2f nextPos = sprite.getPosition();
  nextPos.x += size.x / 2;
  nextLine[0].position = nextPos;
  // if theres no node in front, keep it hidden
  if (!next) {
    nextLine[1].position = nextPos;
  }
}

void LLNode::updateText(const int val) {
  if (val < 0 || val >= 100)
    return;

  ID = val;
  dataText.setString(std::to_string(ID));
}

void LLNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = nullptr;
  target.draw(sprite, states);
  target.draw(dataText, states);
  target.draw(nextLine, states);
}
