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

  // set font
  try {
    if (!font.loadFromFile("./Fonts/arial.ttf"))
      throw std::runtime_error("couldn't load font");
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  dataText.setFont(font);

  // TODO: set the text position properly
  sf::Vector2f textPos = pos;
  textPos.x -= size.x * 0.3;
  textPos.y -= size.y * 0.3;
  dataText.setPosition(textPos);
  dataText.setCharacterSize(size.y/3);
  dataText.setOutlineThickness(1);
  dataText.setOutlineColor(sf::Color::White);
  dataText.setFillColor(sf::Color::Black);
  dataText.setString(std::to_string(data));

  // set texture
  sprite.setTexture(GLOBAL::TREETEXTURE);
  sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
  sprite.setOrigin(size.x * 0.5, size.y * 0.5);
  sprite.setPosition(pos);
  sprite.setColor(sf::Color::White);

  sf::Vector2f leftPos(calcCircleEdge(135));
  leftLine[0].position = leftPos;
  leftLine[1].position = leftPos;
  leftLine[0].color = sf::Color::Yellow;
  leftLine[1].color = sf::Color::Yellow;
  sf::Vector2f rightPos(calcCircleEdge(45));
  rightLine[0].position = rightPos;
  rightLine[1].position = rightPos;
  rightLine[0].color = sf::Color::Yellow;
  rightLine[1].color = sf::Color::Yellow;
}

void TreeNode::updateLine(TreeNode* prev, D direction) {
  if (!prev)
    return;

  // update next line
  sf::Vector2f pos(calcCircleEdge(270));
  if (prev->left && direction == D::LEFT)
    prev->leftLine[1].position = pos;
  if (prev->right && direction == D::RIGHT)
    prev->rightLine[1].position = pos;

  /* update line behind
  if (prevNode) {
    nextPos.x -= size.x;
    prevNode->nextLine[1].position = nextPos;
  }
  */
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