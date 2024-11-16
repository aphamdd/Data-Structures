#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "TreeNode.h"

TreeNode::TreeNode(const int val, const sf::Vector2f pos) :
  data(val),
  left(nullptr),
  right(nullptr),
  leftLine(sf::Lines, 2),
  rightLine(sf::Lines, 2),
  size(88.f, 88.f) {

  // set texture
  sprite.setTexture(GLOBAL::TREETEXTURE);
  sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
  sprite.setOrigin(size.x * 0.5, size.y * 0.5);
  sprite.setPosition(pos);
  sprite.setColor(sf::Color::White);

  // set font
  try {
    if (!font.loadFromFile("./Fonts/arial.ttf"))
      throw std::runtime_error("couldn't load font");
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  dataText.setFont(font);
  dataText.setCharacterSize(30);
  dataText.setOutlineThickness(1);
  dataText.setOutlineColor(sf::Color::White);
  dataText.setFillColor(sf::Color::Black);
  dataText.setString(std::to_string(data));
  sf::FloatRect textRect = dataText.getLocalBounds();
  dataText.setOrigin(textRect.left + textRect.width / 2.0f,
    textRect.top + textRect.height / 2.0f);
  dataText.setPosition(sprite.getPosition());

  // set edges
  sf::Vector2f lPos(calcCircleEdge(135));
  leftLine[0].position = lPos;
  leftLine[1].position = lPos;
  leftLine[0].color = sf::Color::Yellow;
  leftLine[1].color = sf::Color::Yellow;
  sf::Vector2f rPos(calcCircleEdge(45));
  rightLine[0].position = rPos;
  rightLine[1].position = rPos;
  rightLine[0].color = sf::Color::Yellow;
  rightLine[1].color = sf::Color::Yellow;
}

void TreeNode::move(const sf::Vector2f goal) {
  // works but not ideal
  sf::Vector2f textPos = goal;
  textPos.x -= size.x * 0.3;
  textPos.y -= size.y * 0.3;
  dataText.setPosition(textPos);
  sprite.setPosition(goal);
}

void TreeNode::updateLine(TreeNode* prev) {
  // offsetting would be better here using move() instead of repeatedly recalculating
  sf::Vector2f lPos(calcCircleEdge(135));
  sf::Vector2f rPos(calcCircleEdge(45));
  leftLine[0].position = lPos;
  rightLine[0].position = rPos;
  if (!left)
    leftLine[1].position = lPos;
  else {
    sf::Vector2f nextNode(left->sprite.getPosition());
    nextNode.y -= size.y / 2;
    leftLine[1].position = nextNode;
  }

  if (!right)
    rightLine[1].position = rPos;
  else {
    sf::Vector2f nextNode(right->sprite.getPosition());
    nextNode.y -= size.y / 2;
    rightLine[1].position = nextNode;
  }

  // track previous node line
  if (prev) {
    sf::Vector2f current(sprite.getPosition());
    current.y -= size.y / 2;
    if (prev->left.get() == this)
      prev->leftLine[1].position = current;
    else if (prev->right.get() == this)
      prev->rightLine[1].position = current;
  }
}

sf::Vector2f TreeNode::calcCircleEdge(const float angle) {
  // angle goes clockwise instead of counter
  sf::Vector2f pos = sprite.getPosition();
  float theta = angle * (M_PI / 180);
  float x = pos.x + (size.x / 2) * cos(theta);
  float y = pos.y + (size.y / 2) * sin(theta);
  return sf::Vector2f(x, y);
}

void TreeNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = nullptr;
  target.draw(sprite, states);
  target.draw(dataText, states);
  target.draw(leftLine, states);
  target.draw(rightLine, states);
}