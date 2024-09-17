#pragma once
#include "SFML/Graphics.hpp"
#include "Control.h"
#include "common.h"
#include <iostream>

class LLNode : public sf::Drawable, public sf::Transformable {
  friend class LinkedList;
public:
  LLNode(const sf::Vector2f pos);
  ~LLNode();

  int getID() { return ID; };

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public: // make this private? handle solely in the linkedlist class?
  LLNode* next;
  int data;
  int ID;
  sf::RectangleShape shape;
};