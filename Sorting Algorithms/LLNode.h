#pragma once
#include "SFML/Graphics.hpp"
#include "Control.h"
#include "common.h"
#include <iostream>

class LLNode : public sf::Drawable, public sf::Transformable {
public:
  LLNode(const sf::Vector2f pos);
  ~LLNode();

  void add();
  void remove();
  void edit();

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public: 
  LLNode* next;
  int data;
  sf::RectangleShape shape;
};
