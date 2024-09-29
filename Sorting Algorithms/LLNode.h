#pragma once
#include "SFML/Graphics.hpp"
#include "Control.h"
#include "common.h"

// handles the node object and drawing
class LLNode : public sf::Drawable, public sf::Transformable {
  friend class LinkedList;
public:
  LLNode(const sf::Vector2f pos, sf::Text& text, sf::Texture& texture);
  ~LLNode();

  void move(const sf::Vector2f pos);
  void updateLine(LLNode* prev); // updates the next+previous ptr line
  int setText(const int val);

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  LLNode* next = nullptr;
  int data = 1;
  sf::VertexArray nextLine;
  sf::Text dataText;

  sf::Sprite sprite;
  sf::Vector2f size;
  sf::Vector2f shiftFactor;
};
