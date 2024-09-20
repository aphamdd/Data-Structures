#pragma once
#include "SFML/Graphics.hpp"
#include "Control.h"
#include "common.h"
#include <iostream>

// handles the node object and drawing
class LLNode : public sf::Drawable, public sf::Transformable {
  friend class LinkedList;
public:
  LLNode(const sf::Vector2f pos, sf::Text& text);
  ~LLNode();

  void update(const sf::Vector2f mpos, LLNode* prev); // moves node position
  void updateNext(LLNode* prev); // draws the next ptr line
  void updateText(const int val);

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  LLNode* next;
  int data;
  int ID;
  sf::RectangleShape shape;
  sf::VertexArray nextLine;
  sf::Text dataText;
};
