#pragma once
#include "TreeNode.h"

TreeNode::TreeNode(const sf::Vector2f pos, sf::Text& text, sf::Texture& texture) :
  dataText(text),
  leftLine(sf::Lines, 2),
  rightLine(sf::Lines, 2),
  size(88.f, 88.f) {
  sprite.setTexture(texture);
  sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
  sprite.setOrigin(size.x * 0.5, size.y * 0.5);
  sprite.setPosition(pos);
  sprite.setColor(sf::Color::White);

  // TODO: set the text position properly
  sf::Vector2f textPos = pos;
  textPos.x -= size.x * 0.3;
  textPos.y -= size.y * 0.3;
  dataText.setPosition(textPos);
  dataText.setCharacterSize(size.y/2);
  dataText.setFillColor(sf::Color::Black);
  dataText.setString(std::to_string(data));

  // TODO: set up the lines properly
  sf::Vector2f leftPos = pos;
  leftPos.x += size.x * 0.5;
  leftLine[0].position = leftPos;
  leftLine[1].position = leftPos;
  leftLine[0].color = sf::Color::Yellow;
  leftLine[1].color = sf::Color::Yellow;

  sf::Vector2f rightPos = pos;
  rightPos.x += size.x * 0.5;
  rightLine[0].position = rightPos;
  rightLine[1].position = rightPos;
  rightLine[0].color = sf::Color::Yellow;
  rightLine[1].color = sf::Color::Yellow;
}

void TreeNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = nullptr;
  target.draw(sprite, states);
  target.draw(dataText, states);
  target.draw(leftLine, states);
  target.draw(rightLine, states);
}