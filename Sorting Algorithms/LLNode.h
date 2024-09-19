#pragma once
#include "SFML/Graphics.hpp"
#include "Control.h"
#include "common.h"
#include <iostream>

class LLNode : public sf::Drawable, public sf::Transformable {
  friend class LinkedList;
public:
  LLNode(const sf::Vector2f pos, sf::Text& text);
  ~LLNode();

  void update(const sf::Vector2f mpos, LLNode* prev);
  void updateNext(LLNode* prev);

  int getID() { return ID; };

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private: // make this private? handle solely in the linkedlist class?
  LLNode* next;
  int data;
  int ID;
  sf::RectangleShape shape;
  sf::VertexArray nextLine;
  sf::Text dataText;
};
