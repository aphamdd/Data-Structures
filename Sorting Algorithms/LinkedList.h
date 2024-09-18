#pragma once
#include "LLNode.h"

class LinkedList {
public:
  LinkedList(sf::RenderWindow& win);
  ~LinkedList();

  void add();
  bool remove();
  void insert();

  // find node based on shape's current pos relative to mouse pos?
  // think about efficiency: if I've already found the active node, don't search
  LLNode* search(const sf::Vector2i mpos); // find what node im clicking

  void draw() const;


private:
  // TODO: will probably make a vector of LLNode* head ptrs
  LLNode* head;
  LLNode* pActive; // ptr to the current interacted node
  sf::RenderWindow& window; // DI
};
