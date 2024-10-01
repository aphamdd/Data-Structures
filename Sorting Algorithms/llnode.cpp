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
  dataText.setString(std::to_string(data));

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
  data = 0;
}

void LLNode::move(const sf::Vector2f goal) {
  /* can't get this to be bug free and smooth
  // sometimes when I wiggle the shape, it disappears
  sf::Clock clock;
  sf::Vector2f currPos = sprite.getPosition();
  float dx = goal.x - currPos.x;
  float dy = goal.y - currPos.y;
  auto distance = sqrt(pow(dx, 2) + pow(dy, 2)); // euclidean distance formula

  if (distance >= 3) {
    //sf::Vector2f direction(int(dx) >> -1 | 1, int(dy) >> -1 | 1);
    sf::Vector2f direction(dx, dy);
    float deltaTime = clock.restart().asSeconds();
    float k = 10000 * deltaTime;
    float velx = (k * direction.x);
    float vely = (k * direction.y);
    sf::Vector2f offset(velx, vely);
    dataText.move(offset);
    sprite.move(offset);
  }
  */

  // works but not ideal
  sf::Vector2f textPos = goal;
  textPos.x -= size.x / shiftFactor.x;
  textPos.y -= size.y / shiftFactor.y;
  dataText.setPosition(textPos);
  sprite.setPosition(goal);
}

void LLNode::updateLine(LLNode* prevNode) {
  // update next line
  sf::Vector2f nextPos = sprite.getPosition();
  nextPos.x += size.x / 2;
  nextLine[0].position = nextPos;
  // if theres no node in front, keep it hidden
  if (!next) {
    nextLine[1].position = nextPos;
  }

  // update line behind
  if (prevNode) {
    nextPos.x -= size.x;
    prevNode->nextLine[1].position = nextPos;
  }
}

int LLNode::setText(const int val) {
  if (val < 0 || val >= 100) {
    dataText.setString(std::to_string(-1));
    return 0;
  }
  data = val;
  dataText.setString(std::to_string(data));
  return 1;
}

void LLNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = nullptr;
  target.draw(sprite, states);
  target.draw(dataText, states);
  target.draw(nextLine, states);
}
