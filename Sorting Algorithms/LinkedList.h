#pragma once
#include "LLNode.h"

class LinkedList {
public:
  LinkedList(sf::RenderWindow& win, sf::Text& text);
  ~LinkedList();

  void add();
  bool remove();
  void insert();

  LLNode* search(const sf::Vector2i mpos); // find what node im clicking
  bool move(LLNode* p, const sf::Vector2i mpos);
  bool isInBounds(const LLNode* p, const sf::Vector2i mpos) const;

  void draw() const;

private:
  // TODO: will probably make a vector of LLNode* head ptrs
  LLNode* head;
  LLNode* pActive; // ptr to the current interacted node
  sf::RenderWindow& window; // DI
  sf::Text LLText;
};
