#pragma once
#include "SFML/Graphics.hpp"
#include "Control.h"
#include "common.h"
#include <iostream>

// handles the node object and drawing
class LLNode : public sf::Drawable, public sf::Transformable {
  friend class LinkedList;
public:
  LLNode(const sf::Vector2f pos, sf::Text& text, sf::Texture& texture);
  ~LLNode();

  void update(const sf::Vector2f pos, LLNode* prev); // moves node position
  void updateNext(); // draws the next ptr line
  void updateNext(LLNode* prev); // draws the next+previous ptr line
  void updateText(const int val);

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  LLNode* next = nullptr;
  int ID = 1;
  sf::VertexArray nextLine;
  sf::Text dataText;

  sf::Sprite sprite;
  sf::Vector2f size;
  sf::Vector2f shiftFactor;
};
